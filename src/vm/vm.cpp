//
//Created by home on 03/05/25.
//

#include "vm.h"

#include <iomanip>
#include <iostream>

#include "bygen_instructions.h"

VM::VM(const std::vector<uint8_t> &bytecode) : bytecode(bytecode) {
    ip = 0;
    currentCallId = 0;
}

std::string VM::toUtf8(const char32_t c) {
    std::string out;
    if (c <= 0x7F) {
        out.push_back(static_cast<char>(c));
    } else if (c <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | ((c >> 6) & 0x1F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else if (c <= 0xFFFF) {
        out.push_back(static_cast<char>(0xE0 | ((c >> 12) & 0x0F)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | ((c >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
    return out;
}


uint8_t VM::read() {
    if (ip >= bytecode.size()) {
        std::cerr << "Out of bounds at ip = " << ip << ", bytecode.size = " << bytecode.size() << "\n";
        throw std::runtime_error("Out of bounds");
    }
    return bytecode[ip++];
}

ValueT VM::readPayload(const uint8_t type) {
    switch (type) {
        case 0x01: { // i32 (4 bytes)
            uint32_t raw = 0;
            for (int i = 0; i < 4; ++i) {
                raw |= static_cast<uint32_t>(read()) << (8 * i);
            }
            return ValueT{static_cast<int32_t>(raw)};
        }
        case 0x02 : { // i64 (8 bytes)
            uint64_t idx = 0;
            for (int i = 0; i < 8; ++i) {
                idx |= static_cast<int64_t>(read()) << (8 * i);
            }
            return ValueT{static_cast<int64_t>(idx)};
        }
        case 0x03 : {// f32 (4bytes)
            uint32_t raw = 0;
            for (int i = 0; i < 4; ++i) {
                raw |= static_cast<uint32_t>(read()) << (8 * i);
            }
            float val;
            std::memcpy(&val, &raw, sizeof(float));
            return ValueT{val};
        }
        case 0x04: { // f64 (8 bytes)
            uint64_t raw = 0;
            for (int i = 0; i < 8; ++i) {
                raw |= static_cast<uint64_t>(read()) << (8 * i);
            }
            double val;
            std::memcpy(&val, &raw, sizeof(double));
            return ValueT{val};
        }
        case 0x05: { // bool 1 byte
            uint8_t value = read();
            return ValueT{static_cast<bool>(value)};
            }
        case 0x07: { // i8 (1 byte)
            uint8_t value = read();
            return ValueT{static_cast<int8_t>(value)};
        }
        case 0x08: { // i16 (2 bytes)
            uint16_t raw = 0;
            for (int i = 0; i < 2; ++i) {
                raw |= static_cast<uint16_t>(read()) << (8 * i);
            }
            return ValueT{static_cast<int16_t>(raw)};
        }
        case 0x09: { // char (4 bytes utf-32)
            uint32_t raw = 0;
            for (int i = 0; i < 4; ++i) {
                raw |= static_cast<uint32_t>(read()) << (8 * i);
            }
            return ValueT{static_cast<char32_t>(raw)};
        }
        default: {
            return ValueT{};
        }
    }
}

size_t VM::instruLen(const size_t pos) const {
    if (pos + 3 > bytecode.size()) return 0;
    const uint8_t type = bytecode[pos+2];
    size_t payload = 0;
    switch(type) {
        case 0x01: payload = 4; break;
        case 0x02: payload = 8; break;
        case 0x03: payload = 4; break;
        case 0x04: payload = 8; break;
        case 0x05: payload = 1; break;
        case 0x07: payload = 1; break;
        case 0x08: payload = 2; break;
        case 0x09: payload = 4; break;
        default:   payload = 0; break;
    }
    return 3 + payload;
}

ValueT* VM::lookupLocal(int32_t idx) {
    for (int64_t i = callStack.size() - 1; i >= 0 ; --i ) {
        auto& blocks = callStack[i].locals;
        for (int j = static_cast<int>(blocks.size()) - 1; j >= 0; --j) {
            if (blocks[j].contains(idx)) {
                return &blocks[j][idx];
            }
        }
    }
    return nullptr;
}


void VM::registerFunction(size_t &scanIp) {
    if (scanIp + 3 >= bytecode.size()) {
        throw std::runtime_error("Bytecode malformed");
    }

    uint8_t funcId = bytecode[scanIp + 3];
    size_t p = scanIp + instruLen(scanIp);

    if (p < bytecode.size() && bytecode[p] == 0x08) {
        p += instruLen(p);
    }

    size_t countParams = 0;
    while (p < bytecode.size() && bytecode[p] == 0x06) {
        ++countParams;
        p += instruLen(p);
    }

    size_t bodyStart = p;
    size_t q = bodyStart;
    int depth = 1;
    size_t lastQ = q;

    while (q < bytecode.size() && depth > 0) {
        lastQ = q;
        uint8_t cur = bytecode[q];
        if (cur == 0x05) {
            depth++;
            size_t nestedIp = q;
            registerFunction(nestedIp);
        }
        else if (cur == 0x08) ++depth;
        else if (cur == 0x07) --depth;
        q += instruLen(q);
    }

    if (depth != 0) {
        throw std::runtime_error("Bytecode malformed: function body not closed");
    }

    size_t bodyEnd = lastQ + 3;
    functionTable[funcId] = FunctionInfo{ bodyStart, bodyEnd, countParams };

    scanIp = q;
}



void VM::preprocessFunctions() {
    size_t scanIp = 0;

    while (scanIp < bytecode.size()) {
        uint8_t op = bytecode[scanIp];

        if (op == 0x05) {
            registerFunction(scanIp);
        } else {
            scanIp += instruLen(scanIp);
        }
    }
}



void VM::run() {
    callStack.emplace_back();
    callStack.back().locals.emplace_back();
    while (ip < bytecode.size()) {
        const uint8_t opcode = read();
        uint8_t meta = read();
        const uint8_t type = read();
        ValueT payload = readPayload(type);


        switch (opcode) {
            case 0x01: //CONST
                loadStack.emplace(payload);
                break;
            case 0x02: {
                //LOAD
                int32_t idx = std::get<int32_t>(payload);
                ValueT* var = lookupLocal(idx);
                loadStack.push(*var);
                break;
            }
            case 0x03: { // STORE
                int32_t idx = std::get<int32_t>(payload);
                ValueT raw = loadStack.top();
                loadStack.pop();

                ValueT coerced = std::visit([&](auto&& v) -> ValueT {
                    using V = std::decay_t<decltype(v)>;
                    switch (meta) {
                        case 0x01: // i32
                            return ValueT{ static_cast<int32_t>(v) };
                        case 0x02: // i64
                            return ValueT{ static_cast<int64_t>(v) };
                        case 0x03: // f32
                            return ValueT{ static_cast<float>(v) };
                        case 0x04: // f64
                            return ValueT{ static_cast<double>(v) };
                        case 0x05: // bool
                            return ValueT{ static_cast<bool>(v) };
                        case 0x07: // i8
                            return ValueT{ static_cast<int8_t>(v) };
                        case 0x08: // i16
                            return ValueT{ static_cast<int16_t>(v) };
                        case 0x09: // char (UTF-32)
                            return ValueT{ static_cast<char32_t>(v) };
                        default:
                            return ValueT{ v };
                    }
                }, raw);

                ValueT* var = lookupLocal(idx);
                if (var) {
                    *var = coerced;
                } else {
                    callStack.back().locals.back()[idx] = coerced;
                }
            } break;
            case 0x04: //PRINT
                std::visit([]<typename T0>(T0&& val) {
                   using T = std::decay_t<T0>;
                   if constexpr (std::is_same_v<T, bool>) {
                       std::cout << (val ? "true" : "false") << std::endl;
                   }
                   else if constexpr (std::is_same_v<T, int8_t>
                                  || std::is_same_v<T, uint8_t>
                                  || std::is_same_v<T, int16_t>
                                  || std::is_same_v<T, uint32_t>
                                  || std::is_same_v<T, int32_t>
                                  || std::is_same_v<T, uint64_t>
                                  || std::is_same_v<T, int64_t>
                                  || std::is_same_v<T, uint16_t>) {
                       std::cout << static_cast<int64_t>(val) << std::endl;
                   }else if constexpr (std::is_same_v<T, char32_t>) {
                       std::cout << toUtf8(val) << std::endl;
                    }
                   else {
                       std::cout << val << std::endl;
                   }
               }, loadStack.top());
                loadStack.pop();
                break;
            case 0x05: {
                //FN
                ip = functionTable[std::get<int32_t>(payload)].endIp;
                break;
            }
            case 0x07: { // END_BLOCK
                callStack.back().locals.pop_back();
                break;
            }
            case 0x08: { //INIT_BLOCK
                callStack.back().locals.emplace_back();
                break;
            }
            case 0x0E: //ADD
                binaryOp([](auto a, auto b) { return a + b; });
                break;
            case 0x0F: //SUB
                binaryOp([](auto a, auto b) { return a - b; });
                break;
            case 0x10: //MUL
                binaryOp([](auto a, auto b) { return a * b; });
                break;
            case 0x11: //DIV
                binaryOp([](auto a, auto b) {
                    if (b == 0) throw std::runtime_error("Division by zero");
                    return a / b;
                });
                break;
            case 0x13: { // GREATER
                binaryBoolOp([](auto a, auto b) { return a > b; });
                break;
            }
            case 0x14 : { // LESS
                binaryBoolOp([](auto a, auto b) { return a < b; });
                break;
            }
            case 0x15: { // EQUAL_EQUAL
                binaryBoolOp([](auto a, auto b) { return a == b; });
                break;
            }
            case 0x16: {//CALL
                currentCallId = std::get<int32_t>(payload);
                break;
            }
            case 0x17 : { //CALL_END
                auto& funcInfo = functionTable[currentCallId];
                CallFrame frame;
                frame.ip = funcInfo.startIp;
                frame.returnIp = ip;
                frame.locals.emplace_back();
                for (size_t i = 0; i < funcInfo.params; i++) {
                    frame.locals.back()[i] = loadStack.top();
                    loadStack.pop();
                }

                callStack.push_back(frame);
                ip = funcInfo.startIp;
                break;
            }
            case 0x18: { // RET
                ValueT retVal{};
                if (!loadStack.empty() && type != 0x00) {
                    retVal = loadStack.top();
                    loadStack.pop();
                }

                CallFrame finished = callStack.back();
                callStack.pop_back();

                ip = finished.returnIp;

                if (type != 0x00) loadStack.emplace(retVal);

                if (ip < bytecode.size() && bytecode[ip] == 0x07) {
                    ip += instruLen(ip);
                }
                break;
            }

            case 0x1A : { // IF_FALSE
                ValueT val = loadStack.top();
                loadStack.pop();
                bool cond = std::visit([](auto&& x) -> bool {
                    using T = std::decay_t<decltype(x)>;
                    if constexpr (std::is_same_v<T, bool>) return x;
                    else return static_cast<bool>(x);
                }, val);
                if (!cond) {
                    ip += std::get<int32_t>(payload);
                }
                break;
            }
            case 0x1B: { // JMP
                ip += std::get<int32_t>(payload);
                break;
            }
            case 0x1C :  { // NEG
                auto val = loadStack.top();
                loadStack.pop();
                loadStack.emplace(std::visit([] (auto a) -> ValueT {
                    return -a;
                }, val));
                break;
            }
            case 0x1D: { // INC
                int32_t idx = std::get<int32_t>(payload);
                ValueT* slot = lookupLocal(idx);
                std::visit([](auto& x){ if constexpr(std::is_integral_v<std::decay_t<decltype(x)>> && !std::is_same_v<std::decay_t<decltype(x)>, bool>) ++x; }, *slot);
                loadStack.emplace(*slot);
                break;
            }
            case 0x1E: { // DEC
                int32_t idx = std::get<int32_t>(payload);
                ValueT* slot = lookupLocal(idx);
                std::visit([](auto& x){ if constexpr(std::is_integral_v<std::decay_t<decltype(x)>> && !std::is_same_v<std::decay_t<decltype(x)>, bool>) --x; }, *slot);
                loadStack.emplace(*slot);
                break;
                break;
            }
            case 0x1F: { // POST_INC
                int32_t idx = std::get<int32_t>(payload);
                ValueT* slot = lookupLocal(idx);
                ValueT old = *slot;
                std::visit([](auto& x) {
                    using T = std::decay_t<decltype(x)>;
                    if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                        ++x;
                    }
                }, *slot);
                loadStack.emplace(old);
                break;
            }
            case 0x20: { // POST_DEC
                int32_t idx = std::get<int32_t>(payload);
                ValueT* slot = lookupLocal(idx);
                ValueT old = *slot;
                std::visit([](auto& x) {
                    using T = std::decay_t<decltype(x)>;
                    if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                        --x;
                    }
                }, *slot);
                loadStack.emplace(old);
                break;
            }
            case 0x21 : { // GREATER_EQUAL
                binaryBoolOp([](auto a, auto b) { return a >= b; });
                break;
            }
            case 0x22  : { // LESS_EQUAL
                binaryBoolOp([](auto a, auto b) { return a <= b; });
                break;
            }
            case 0x23 : {  // NOT_EQUAL
                binaryBoolOp([](auto a, auto b) { return a != b; });
                break;
            }
            case 0x24: {  // AND
                binaryBoolOp([](const ValueT& a, const ValueT& b) {
                    return std::visit([](auto va, auto vb) {
                        return va && vb;
                    }, a, b);
                });
                break;
            }
            case 0x25: { // OR
               binaryBoolOp([](const ValueT& a, const ValueT& b) {
                    return std::visit([](auto va, auto vb) {
                        return va || vb;
                    }, a, b);
                });
                break;
            }
            case 0x26: { //NOT
                auto val = loadStack.top();
                loadStack.pop();
                loadStack.emplace(static_cast<bool>(std::visit([](auto a) {
                    return !a;
                }, val)));
                break;
            }
            case 0x27: // BIT_AND
                bitwiseOp([](auto x, auto y) { return x & y; });
                break;
            case 0x28: // BIT_OR
                bitwiseOp([](auto x, auto y) { return x | y; });
                break;
            case 0x29: // BIT_XOR
                bitwiseOp([](auto x, auto y) { return x ^ y; });
                break;
            case 0x2A: // SHL
                bitwiseOp([](auto x, auto y) { return x << y; });
                break;
            case 0x2B: // SHR
                bitwiseOp([](auto x, auto y) { return x >> y; });
                break;
            case 0x2C: { // BIT_NOT
                ValueT val = loadStack.top();
                loadStack.pop();
                ValueT result = std::visit([](auto x) -> ValueT {
                    using T = std::decay_t<decltype(x)>;
                    if constexpr (std::is_integral_v<T> && !std::is_same_v<T, char32_t>) {
                        return ~x;
                    } else {
                        throw std::runtime_error("BIT_NOT: operand must be integral");
                    }
                }, val);

                loadStack.push(result);
                break;
            }

        }
    }
}
