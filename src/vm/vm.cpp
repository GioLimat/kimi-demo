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
        case 0x04: { // f64 (8 bytes)
            uint64_t raw = 0;
            for (int i = 0; i < 8; ++i) {
                raw |= static_cast<uint64_t>(read()) << (8 * i);
            }
            double val;
            std::memcpy(&val, &raw, sizeof(double));
            return ValueT{val};
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
        case 0x04: payload = 8; break;
        default:   payload = 0; break;
    }
    return 3 + payload;
}

ValueT& VM::lookupLocal(const int32_t idx) const {
    std::stack<CallFrame> temp = callStack;
    while (!temp.empty()) {
        auto& blocks = temp.top().locals;
        for (int i = static_cast<int>(blocks.size()) - 1; i >= 0; --i) {
            if (blocks[i].contains(idx)) {
                return blocks[i][idx];
            }
        }
        temp.pop();
    }
    throw std::runtime_error("Variable not found " + std::to_string(idx));
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
    callStack.emplace();
    callStack.top().locals.emplace_back();
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
                loadStack.emplace(lookupLocal(idx));
                break;
            }
            case 0x03:  { // STORE
                int32_t idx = std::get<int32_t>(payload);
                ValueT val = loadStack.top();
                loadStack.pop();

                callStack.top().locals.back()[idx] = val;
                break;
            }
            case 0x04: //PRINT
                switch (type) {
                    case 0x01: //i32
                        std::cout << std::get<int32_t>(loadStack.top()) << std::endl;
                        break;
                    case 0x04:  //F64
                        std::cout << std::get<double>(loadStack.top()) << std::endl;
                        break;
                    case 0x05:
                        std::cout << (std::get<int32_t>(loadStack.top()) ? "true" : "false") << std::endl;
                        break;
                }
                loadStack.pop();
                break;
            case 0x05: {
                //FN
                ip = functionTable[std::get<int32_t>(payload)].endIp;
                break;
            }
            case 0x07: { // END_BLOCK
                callStack.top().locals.pop_back();
                break;
            }
            case 0x08: { //INIT_BLOCK
                callStack.top().locals.emplace_back();
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
                ValueT a = loadStack.top();
                loadStack.pop();
                ValueT b = loadStack.top();
                loadStack.pop();
                switch (type) {
                    case 0x01:
                        loadStack.emplace(std::get<int32_t>(a) < std::get<int32_t>(b) ? 1 : 0);
                        break;
                    case 0x04:
                        loadStack.emplace(std::get<double>(a) < std::get<double>(b) ? 1 : 0);
                        break;
                }
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

                callStack.emplace(frame);
                ip = funcInfo.startIp;
                break;
            }
            case 0x18: { //RET
                ValueT retVal{};
                if (!loadStack.empty()) {
                    retVal = loadStack.top();
                    loadStack.pop();
                }

                CallFrame finished = callStack.top();
                callStack.pop();

                ip = finished.returnIp;

                loadStack.emplace(retVal);
                break;
            }
            case 0x1A : { // IF_FALSE
                ValueT val = loadStack.top();
                loadStack.pop();
                if (auto boolean = std::get<int32_t>(val); boolean == 0) {
                    ip += std::get<int32_t>(payload);
                }
                break;
            }
            case 0x1B: { // JMP
                ip += std::get<int32_t>(payload);
                break;
            }
            case 0x1C :  { // NEG
                ValueT val = loadStack.top();
                loadStack.pop();
                switch (type) {
                    case 0x01:
                        loadStack.emplace(-std::get<int32_t>(val));
                        break;
                    case 0x04:
                        loadStack.emplace(-std::get<double>(val));
                        break;
                }
            }
            case 0x1D: { // INC
                auto & dest = callStack.top().locals.back();

                const int32_t idx = std::get<int32_t>(payload);
                ValueT &val = dest.at(idx);

                switch (type) {
                    case 0x01: {
                        auto &v = std::get<int32_t>(val);
                        ++v;
                        loadStack.emplace(v);
                        break;
                    }
                    case 0x04: {
                        auto &v = std::get<double>(val);
                        ++v;
                        loadStack.emplace(v);
                        break;
                    }
                }
                break;
            }
            case 0x1E: { // INC
                auto & dest = callStack.top().locals.back();

                int32_t idx = std::get<int32_t>(payload);
                ValueT &val = dest.at(idx);

                switch (type) {
                    case 0x01: {
                        auto &v = std::get<int32_t>(val);
                        --v;
                        loadStack.emplace(v);
                        break;
                    }
                    case 0x04: {
                        auto &v = std::get<double>(val);
                        --v;
                        loadStack.emplace(v);
                        break;
                    }
                }
                break;
            }
            case 0x1F: { // POST_INC
                auto & dest = callStack.top().locals.back();
                int32_t idx = std::get<int32_t>(payload);
                ValueT& val = dest.at(idx);
                switch (type) {
                    case 0x01: {
                        auto& v = std::get<int32_t>(val);
                        loadStack.emplace(v);
                        ++v;
                        break;
                    }
                    case 0x04: {
                        auto& v = std::get<double>(val);
                        loadStack.emplace(v);
                        ++v;
                        break;
                    }
                }
                break;
            }
            case 0x20: { // POST_DEC
                auto & dest = callStack.top().locals.back();
                int32_t idx = std::get<int32_t>(payload);
                ValueT& val = dest.at(idx);
                switch (type) {
                    case 0x01: {
                        auto& v = std::get<int32_t>(val);
                        loadStack.emplace(v);
                        --v;
                        break;
                    }
                    case 0x04: {
                        auto& v = std::get<double>(val);
                        loadStack.emplace(v);
                        --v;
                        break;
                    }
                }
                break;
            }

        }

    }
}
