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

    static std::string toUtf8(char32_t c);
    uint8_t read();
    ValueT readPayload(uint8_t type);
    size_t instruLen(size_t pos) const;
    void registerFunction(size_t& scanIp);
    ValueT* lookupLocal(int32_t idx);

    template<typename Func>
    void binaryOp(Func op) {
        if (loadStack.size() < 2) return;

        ValueT rhs = loadStack.top(); loadStack.pop();
        ValueT lhs = loadStack.top(); loadStack.pop();

        ValueT result = std::visit([&](auto a, auto b) -> ValueT {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;

            auto convert = []<typename T0>(T0 val) {
                if constexpr (std::is_same_v<T0, char32_t>)
                    return static_cast<int32_t>(val);
                else
                    return val;
            };

            auto ca = convert(a);
            auto cb = convert(b);
            auto native = op(ca, cb);

            if constexpr (std::is_same_v<A, char32_t> || std::is_same_v<B, char32_t>) {
                return ValueT{ static_cast<char32_t>(native) };
            }
            else if constexpr (std::is_integral_v<decltype(native)> && !std::is_same_v<decltype(native), bool>) {
                return ValueT{ static_cast<int32_t>(native) };
            }
            else {
                return ValueT{ native };
            }
        }, lhs, rhs);

        loadStack.push(result);
    }



    template<typename Func>
    inline void binaryBoolOp(Func&& op) {
            ValueT rhs = loadStack.top(); loadStack.pop();
            ValueT lhs = loadStack.top(); loadStack.pop();

            bool result = std::visit(
                [&](auto&& a, auto&& b) -> bool {
                    using A = std::decay_t<decltype(a)>;
                    using B = std::decay_t<decltype(b)>;
                    if constexpr (std::is_arithmetic_v<A> && std::is_arithmetic_v<B>) {
                        return op(static_cast<double>(a), static_cast<double>(b));
                    } else {
                        return op(a, b);
                    }
                },
                lhs, rhs
            );

            loadStack.emplace(result);
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
