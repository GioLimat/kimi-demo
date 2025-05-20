//
// Created by home on 03/05/25.
//

#ifndef VM_H
#define VM_H
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <bits/ostream.tcc>

#include "value.h"

class VM {
    struct CallFrame {
        std::vector<std::unordered_map<size_t, ValueT>> locals;
        size_t returnIp;
        size_t ip;
    };
    struct FunctionInfo {
        size_t startIp;
        size_t endIp;
        size_t params;
    };
public:
    explicit VM(const std::vector<uint8_t>& bytecode);

    void run();
    void preprocessFunctions();
private:
    std::vector<uint8_t> bytecode;
    size_t ip;
    size_t currentCallId;
    std::vector<CallFrame> callStack;
    std::stack<ValueT> loadStack;
    std::unordered_map<size_t, FunctionInfo> functionTable;

    uint8_t read();
    ValueT readPayload(uint8_t type);
    size_t instruLen(size_t pos) const;
    void registerFunction(size_t& scanIp);
    ValueT* lookupLocal(int32_t idx);

    template<typename Func>
    void binaryOp(Func op) {
            ValueT rhs = loadStack.top(); loadStack.pop();
            ValueT lhs = loadStack.top(); loadStack.pop();

            const ValueT result = std::visit([&]<typename T0, typename T1>(T0 a, T1 b) -> ValueT {
                return op(a, b);
            }, lhs, rhs);

            loadStack.push(result);
    }

    template<typename Func>
    inline void binaryBoolOp(Func&& op) {
            ValueT rhs = loadStack.top(); loadStack.pop();
            ValueT lhs = loadStack.top(); loadStack.pop();

            loadStack.emplace(static_cast<bool>(std::forward<Func>(op)(lhs, rhs)));
        }


    template<typename Func>
void bitwiseOp(Func op) {
        ValueT rhs = loadStack.top(); loadStack.pop();
        ValueT lhs = loadStack.top(); loadStack.pop();

        ValueT result = std::visit([&](auto a, auto b) -> ValueT {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            if constexpr (std::is_integral_v<A> && std::is_integral_v<B>) {
                using T = std::common_type_t<A, B>;
                return static_cast<T>(op(static_cast<T>(a), static_cast<T>(b)));
            } else {
                throw std::runtime_error("Bitwise op requires integer types");
            }
        }, lhs, rhs);

        loadStack.push(result);
    }

};

#endif //VM_H
