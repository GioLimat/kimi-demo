//
// Created by home on 26/05/25.
//

#include <iostream>
#include <ranges>

#include "orion_vm.h"



uint64_t OrionVM::decodeType(const uint8_t type) {
    switch (type) {
        case 0x01: return 4;
        case 0x02: return 8;
        case 0x03: return 4;
        case 0x04: return 8;
        case 0x05: return 1;
        case 0x06: return 0;
        case 0x07: return 1;
        case 0x08: return 2;
        case 0x09: return 4;
        default:   return 0;
    }
}


uint64_t OrionVM::getInstructionLength(uint8_t op) {
    switch (op) {
        case 0x01:
            return 2 + decodeType(bytecode[ip + 1]); // 1 byte for opcode, 1 byte for type, plus type size
        case 0x02:  // LOAD
        case 0x1D:  // INC
        case 0x1E:  // DEC
        case 0x1F:  // POST_INC
        case 0x20:  // POST_DEC
        case 0x03:
            return 7;
        case 0x1A:
        case 0x1B:
        case 0x05:
        case 0x16:
        case 0x06:
            return 5; // 1 byte for opcode, 4 bytes for offset
        case 0x07:
        case 0x08:
        case 0x19:
            return 1; // END_BLOCK and INIT_BLOCK are 1 byte each
        case 0x04: // PRINT
        case 0x0E: // ADD
        case 0x0F: // SUB
        case 0x10: // MUL
        case 0x11: // DIV
        case 0x12: // MOD
        case 0x13: // GREATER
        case 0x14: // LESS
        case 0x15: // EQUAL
        case 0x18: //  RET
        case 0x1C: // NEG
        case 0x21: // GREATER_EQUAL
        case 0x22: // LESS_EQUAL
        case 0x23: // NOT_EQUAL
        case 0x24: // AND
        case 0x25: // OR
        case 0x26: // NOT
        case 0x27: // BIT_AND
        case 0x28: // BIT_OR
        case 0x29: // BIT_XOR
        case 0x2A: // SHIFT_LEFT
        case 0x2B: // SHIFT_RIGHT
        case 0x2C: // BIT_NOT
            return 2;
        case 0x00: //HALT
            return 1;
        default:
            return 0;
    }
}



void OrionVM::preprocessFunctions() {
#if DEBUG
    uint64_t countFunctions = 0;
#endif
    while (ip < bytecode.size()) {
        if (bytecode[ip] == 0x05) {// FN
#if DEBUG
            countFunctions++;
#endif
            ip += 5; // Skip FN opcode and 4-byte identifier

            uint64_t returnIp = ip; // Save the position to return to after function declaration

            registerFunction(&returnIp);

            ip = returnIp;
        }
        else ip += getInstructionLength(bytecode[ip]);
    }

#if DEBUG
    std::cout << "Preprocessing functions: " << countFunctions << " functions found." << std::endl;
#endif
    ip = 0; // Reset instruction pointer after preprocessing
}


void OrionVM::registerFunction(uint64_t* returnPos) {
    uint64_t startIp = ip;
    uint64_t endIp   = 0;
    uint64_t args    = 0;

    FunctionInfo placeholder{};
    placeholder.initIp   = startIp;
    placeholder.endIp    = 0;
    placeholder.argsSize = 0;
    functions.push_back(placeholder);

    size_t myIndex = functions.size() - 1;

    while (ip < bytecode.size()) {
        uint8_t op = bytecode[ip];

        if (op == 0x05) {
            ip += 5;
            uint64_t nestedEnd;
            registerFunction(&nestedEnd);
            ip = nestedEnd + 1;
            continue;
        }

        if (op == 0x06) {
            args++;
        } else if (op == 0x19) {
            endIp = ip;
            break;
        }

        ip += getInstructionLength(op);
    }

    functions[myIndex].endIp    = endIp;
    functions[myIndex].argsSize = args;
    *returnPos = endIp;

#if DEBUG
    std::cout << "Registered function at IP: " << startIp
              << ", end IP: " << endIp
              << ", args size: " << args
              << std::endl;
#endif
}
