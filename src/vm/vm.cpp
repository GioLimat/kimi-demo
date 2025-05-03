//
//Created by home on 03/05/25.
//

#include "vm.h"

#include <iostream>

#include "bygen_instructions.h"

VM::VM(const std::vector<uint8_t> &bytecode) : bytecode(bytecode) {
    ip = 0;
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
            case 0x03: //STORE
                callStack.top().locals[std::get<int32_t>(payload)] = loadStack.top();
                loadStack.pop();
                break;
            case 0x04: //PRINT
                std::cout << std::get<int32_t>(loadStack.top()) << std::endl;
                loadStack.pop();
                break;
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
        }
    }
}
