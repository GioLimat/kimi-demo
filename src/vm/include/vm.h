//
// Created by home on 03/05/25.
//

#ifndef VM_H
#define VM_H
#include <cstdint>
#include <cstring>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <vector>

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
    std::stack<CallFrame> callStack;
    std::stack<ValueT> loadStack;
    std::unordered_map<size_t, FunctionInfo> functionTable;

    uint8_t read();
    ValueT readPayload(uint8_t type);
    size_t instruLen(size_t pos) const;
    void registerFunction(size_t& scanIp);
    ValueT& lookupLocal(int32_t idx) const;

    template<typename Func>
    void binaryOp(Func op) {
            ValueT rhs = loadStack.top(); loadStack.pop();
            ValueT lhs = loadStack.top(); loadStack.pop();

            const ValueT result = std::visit([&]<typename T0, typename T1>(T0 a, T1 b) -> ValueT {
                using A = T0;
                using B = T1;

                if constexpr (std::is_same_v<A, B>) {
                    return op(a, b);
                } else {
                    throw std::runtime_error("Type mismatch in binary operation");
                }
            }, lhs, rhs);

            loadStack.push(result);
    }
};

#endif //VM_H
