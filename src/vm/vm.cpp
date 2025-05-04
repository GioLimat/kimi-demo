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
        throw std::runtime_error("Out of bounds");
    }
    return bytecode[ip++];
}

ValueT VM::readPayload(const uint8_t type) {
    switch (type) {
        case 0x01: return ValueT{read()};
        default: return ValueT{};
    }
}

void VM::preprocessFunctions() {
    size_t scanIp = 0;
    while (scanIp < bytecode.size()) {
        uint8_t op = bytecode[scanIp];
        if (op == 0x05) {
            uint8_t funcId = bytecode[scanIp + 3];

            size_t ip0 = scanIp + 4;
            size_t countParams = 0;
            while (ip0 < bytecode.size() && bytecode[ip0] == 0x06) {
                countParams++;
                ip0 += 4;
            }
            size_t bodyStart = ip0;

            size_t ip1 = bodyStart;
            int depth = 1;
            while (ip1 < bytecode.size() && depth > 0) {
                uint8_t cur = bytecode[ip1];
                if (cur == 0x05) depth++;
                else if (cur == 0x07) depth--;
                ip1 += 4;
            }
            size_t bodyEnd = ip1 - 4;
            functionTable[funcId] = FunctionInfo{ bodyStart, bodyEnd, countParams };
            scanIp = ip1;
        } else {
            scanIp += 4;

    }
    }
}


void VM::run() {
    callStack.emplace();
    callStack.top().locals.reserve(10);
    while (ip < bytecode.size()) {
        const uint8_t opcode = read();
        uint8_t meta = read();
        const uint8_t type = read();
        ValueT payload = readPayload(type);


        switch (opcode) {
            case 0x01: //CONST
                loadStack.emplace(payload);
                break;
            case 0x02: //LOAD
                loadStack.emplace(callStack.top().locals.at(std::get<int32_t>(payload)));
                break;
            case 0x03:  { // STORE
                const int32_t index = std::get<int32_t>(payload);
                auto& locals = callStack.top().locals;
                if (index >= locals.size()) {
                    locals.resize(index + 1);
                }
                locals[index] = loadStack.top();
                loadStack.pop();
                break;
            }
            case 0x04: //PRINT
                std::cout << std::get<int32_t>(loadStack.top()) << std::endl;
                loadStack.pop();
                break;
            case 0x05: {
                //FN
                ip = functionTable[std::get<int32_t>(payload)].endIp + 4;
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
            case 0x16: {//CALL
                currentCallId = std::get<int32_t>(payload);
                break;
            }
            case 0x17 : { //CALL_END
                auto& funcInfo = functionTable[currentCallId];
                CallFrame frame;
                std::cout << "CALLING" << std::endl;
                frame.ip = funcInfo.startIp;
                frame.returnIp = ip + 4;
                frame.locals.resize(funcInfo.params);

                for (size_t i = 0; i < funcInfo.params; i++) {
                    frame.locals[i] = loadStack.top();
                    loadStack.pop();
                }

                callStack.emplace(frame);

                ip = funcInfo.startIp;
                break;
            }
        }

    }
}
