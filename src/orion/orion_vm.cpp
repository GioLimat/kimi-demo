//
// Created by home on 22/05/25.
//

#include "orion_vm.h"

#include <cstring>
#include <iostream>

#include "add_handlers.h++"
#include "div_handlers.h++"
#include "mod_handlers.h++"
#include "mul_handlers.h++"
#include "print_handlers.h++"
#include "sub_handlers.h++"

OrionVM::OrionVM(const std::vector<uint8_t> &bytecode) : bytecode(bytecode) {}

RawValue OrionVM::readPayload(const uint8_t type) {
    switch (type) {
        case 0x00: return 0;
        case 0x01: { // i32 (4 bytes)
            uint32_t r = 0;
            for (int i = 0; i < 4; ++i) r |= static_cast<uint32_t>(read()) << (8 * i);
            return static_cast<RawValue>(static_cast<int32_t>(r));
        }
        case 0x02: { // i64 (8 bytes)
            uint64_t r = 0;
            for (int i = 0; i < 8; ++i) r |= static_cast<uint64_t>(read()) << (8 * i);
            return r;
        }
        case 0x03: { // f32 (4 bytes)
            uint32_t raw = 0;
            for (int i = 0; i < 4; ++i) raw |= static_cast<uint32_t>(read()) << (8 * i);
            float f;
            std::memcpy(&f, &raw, sizeof(f));
            uint64_t bits;
            std::memcpy(&bits, &f, sizeof(f));
            return bits;
        }
        case 0x04: { // f64 (8 bytes)
            uint64_t raw = 0;
            for (int i = 0; i < 8; ++i) raw |= static_cast<uint64_t>(read()) << (8 * i);
            return raw;
        }
        case 0x05: // bool (1 byte)
            return read() != 0;
        case 0x07: // i8 (1 byte)
            return static_cast<RawValue>(static_cast<int8_t>(read()));
        case 0x08: { // i16 (2 bytes)
            uint16_t r = 0;
            for (int i = 0; i < 2; ++i) r |= static_cast<uint16_t>(read()) << (8 * i);
            return static_cast<RawValue>(static_cast<int16_t>(r));
        }
        case 0x09: { // char (4 bytes UTF-32)
            uint32_t r = 0;
            for (int i = 0; i < 4; ++i) r |= static_cast<uint32_t>(read()) << (8 * i);
            return r;
        }
        default:
            return 0;
    }
}

void OrionVM::run() {
    callStack.push_back({0, 0, nullptr});

    static void* dispatchTable[MAX_OPCODE] = {nullptr};

#define OPCODE_CASE(op, label) dispatchTable[op] = &&label;
#define DISPATCH() goto *dispatchTable[read()];

    OPCODE_CASE(0x00, L_HALT);
    OPCODE_CASE(0x01, L_CONST);
    OPCODE_CASE(0x02, L_LOAD);
    OPCODE_CASE(0x03, L_STORE);
    OPCODE_CASE(0x04, L_PRINT);
    OPCODE_CASE(0x0E, L_ADD);
    OPCODE_CASE(0x0F, L_SUB);
    OPCODE_CASE(0x10, L_MUL);
    OPCODE_CASE(0x11, L_DIV);
    OPCODE_CASE(0x12, L_MOD);


    DISPATCH();


    L_CONST: {
#if DEBUG
        opcodeCount[0x01]++;
#endif
        uint8_t type = read();
        push(readPayload(type));
        DISPATCH();
    }

    L_LOAD: {
#if DEBUG
        opcodeCount[0x02]++;
#endif
        uint8_t meta = read();
        uint32_t varId = read32();

        uint8_t callDepth = meta >> 4;

        CallFrame* target = &callStack.back();
        for (uint8_t i = 0; i < callDepth; ++i) target = target->parent;
        RawValue value = stackBuf[target->stackBase + varId];
        push(value);
        DISPATCH();
    }


    L_STORE: {
#if DEBUG
        opcodeCount[0x03]++;
#endif
        uint32_t varId = read32();

        RawValue value = pop();
        stackBuf[callStack.back().stackBase + varId] = value;
        sp = callStack.back().stackBase + varId + 1;
        DISPATCH();
    }


    L_PRINT: {
#if DEBUG
        opcodeCount[0x04]++;
#endif
        uint8_t type = read();
        RawValue v = pop();
        if (auto fn = printTable[type]) fn(v);
        else std::printf("PRINT: unsupported type 0x%02X\n", type);
        DISPATCH();
    }


    L_ADD: {
#if DEBUG
        opcodeCount[0x0E]++;
#endif
        uint8_t type = read();
        addTable[type](*this);
        DISPATCH();
    }


    L_SUB: {
#if DEBUG
        opcodeCount[0x0F]++;
#endif
        uint8_t type = read();
        subTable[type](*this);
        DISPATCH();
    }


    L_MUL: {
#if DEBUG
        opcodeCount[0x10]++;
#endif
        uint8_t type = read();
        mulTable[type](*this);
        DISPATCH();
    }


    L_DIV: {
#if DEBUG
        opcodeCount[0x11]++;
#endif
        uint8_t type = read();
        divTable[type](*this);
        DISPATCH();
    }

    L_MOD: {
    #if DEBUG
        opcodeCount[0x12]++;
    #endif
        uint8_t type = read();
        modTable[type](*this);
        DISPATCH();
    }



    L_HALT: {
    #if DEBUG
        opcodeCount[0x00]++;
        std::cout << "\n\n\n-------LOGGING-------" << std::endl;
        for (size_t i = 0; i < MAX_OPCODE; ++i) {
            if (opcodeCount[i] > 0) {
                printf("Opcode 0x%02zx executed %llu times\n", i, opcodeCount[i]);
            }
        }
    #endif
        return;
    }
#undef DISPATCH
}
