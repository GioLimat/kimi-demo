//
// Created by home on 22/05/25.
//

#include "orion_vm.h"

#include <cstring>
#include <iostream>

#include "add_handlers.h++"
#include "bit_and_handlers.h++"
#include "bit_not_handlers.h++"
#include "bit_or_handlers.h++"
#include "bit_sl_handlers.h++"
#include "bit_sr_handlers.h++"
#include "bit_xor_handlers.h++"
#include "div_handlers.h++"
#include "equal_handlers.h++"
#include "ge_handlers.h++"
#include "greater_handlers.h++"
#include "less_handlers.h++"
#include "le_handlers.h++"
#include "mod_handlers.h++"
#include "mul_handlers.h++"
#include "neg_handlers.h++"
#include "ne_handlers.h++"
#include "not_handlers.h++"
#include "print_handlers.h++"
#include "sub_handlers.h++"
#include "logical/and_handlers.h++"
#include "logical/or_handlers.h++"

OrionVM::OrionVM(const std::vector<uint8_t> &bytecode) : bytecode(bytecode) {}


void OrionVM::run() {
    callStack.push_back({0, 0, nullptr});
    callStack.back().scopeSp.push_back(0);

    static void* dispatchTable[MAX_OPCODE] = {nullptr};

#define OPCODE_CASE(op, label) dispatchTable[op] = &&label;
#define DISPATCH() goto *dispatchTable[read()];

    /*NO MEANING JUST READ ERASE AT THE FUTURE*/
    OPCODE_CASE(0x06, L_FN_PARAM);
    OPCODE_CASE(0x19, L_END_FN)

    OPCODE_CASE(0x00, L_HALT);
    OPCODE_CASE(0x01, L_CONST);
    OPCODE_CASE(0x02, L_LOAD);
    OPCODE_CASE(0x03, L_STORE);
    OPCODE_CASE(0x04, L_PRINT);
    OPCODE_CASE(0x05, L_FN);
    OPCODE_CASE(0x07, L_END_BLOCK);
    OPCODE_CASE(0x08, L_INIT_BLOCK);
    OPCODE_CASE(0x0E, L_ADD);
    OPCODE_CASE(0x0F, L_SUB);
    OPCODE_CASE(0x10, L_MUL);
    OPCODE_CASE(0x11, L_DIV);
    OPCODE_CASE(0x12, L_MOD);
    OPCODE_CASE(0x13, L_GREATER);
    OPCODE_CASE(0x14, L_LESS);
    OPCODE_CASE(0x15, L_EQUAL);
    OPCODE_CASE(0x16, L_CALL);
    OPCODE_CASE(0x18, L_RET);
    OPCODE_CASE(0x1A, L_IF_FALSE);
    OPCODE_CASE(0x1B, L_JMP);
    OPCODE_CASE(0x1C, L_NEG);
    OPCODE_CASE(0x1D, L_INC);
    OPCODE_CASE(0x1E, L_DEC);
    OPCODE_CASE(0x1F, L_POST_INC);
    OPCODE_CASE(0x20, L_POST_DEC);
    OPCODE_CASE(0x21, L_GREATER_EQUAL);
    OPCODE_CASE(0x22, L_LESS_EQUAL);
    OPCODE_CASE(0x23, L_NOT_EQUAL);
    OPCODE_CASE(0x24, L_AND);
    OPCODE_CASE(0x25, L_OR);
    OPCODE_CASE(0x26, L_NOT);
    OPCODE_CASE(0x27, L_BIT_AND);
    OPCODE_CASE(0x28, L_BIT_OR);
    OPCODE_CASE(0x29, L_BIT_XOR);
    OPCODE_CASE(0x2A, L_SHIFT_LEFT);
    OPCODE_CASE(0x2B, L_SHIFT_RIGHT);
    OPCODE_CASE(0x2C, L_BIT_NOT);

    DISPATCH();


    L_END_FN: {
        DISPATCH();
    }

    L_FN_PARAM:{
        read32();
        DISPATCH();
    }


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
        uint8_t callFrame = read();
        uint8_t scope = read();
        uint32_t varId = read32();

        CallFrame* targetFrame = &callStack[callStack.size() - callFrame - 1];

        uint64_t targetScope = targetFrame->scopeSp[targetFrame->scopeSp.size() - scope - 1];

        RawValue value = stackBuf[targetScope + varId];
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


    L_FN: {
#if DEBUG
        opcodeCount[0x05]++;
#endif
        int32_t fnId = readSigned32();

        ip = functions[fnId].endIp;

        DISPATCH();
    }



    L_END_BLOCK: {
#if DEBUG
        opcodeCount[0x07]++;
#endif
        sp = callStack.back().scopeSp.back();
        callStack.back().scopeSp.pop_back();
        DISPATCH();
    }

    L_INIT_BLOCK: {
#if DEBUG
        opcodeCount[0x08]++;
#endif
        callStack.back().scopeSp.push_back(sp);
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



    L_GREATER: {
#if DEBUG
        opcodeCount[0x13]++;
#endif
        uint8_t type = read();
        greaterTable[type](*this);
        DISPATCH();
    }

    L_LESS: {
#if DEBUG
        opcodeCount[0x14]++;
#endif
        uint8_t type = read();
        lessTable[type](*this);
        DISPATCH();
    }


    L_EQUAL: {
#if DEBUG
        opcodeCount[0x15]++;
#endif
        uint8_t type = read();

        equalTable[type](*this);
        DISPATCH();
    }



    L_CALL: {
#if DEBUG
        opcodeCount[0x16]++;
#endif
        int32_t fnId = readSigned32();
        const auto fn = functions[fnId];

        CallFrame frame{};
        frame.scopeSp.push_back(sp - fn.argsSize);
        frame.returnIp = ip;
        frame.parent = &callStack.back();
        frame.stackBase = sp - fn.argsSize;
        ip = fn.initIp;
        for (uint64_t i = 0; i < fn.argsSize; ++i) {
            stackBuf[sp++] = pop();
        }

        callStack.push_back(frame);
        DISPATCH();
    }



    L_RET: {
#if DEBUG
        opcodeCount[0x18]++;
#endif
        uint8_t type = read();
        ip = callStack.back().returnIp;
        RawValue value = pop();
        sp = callStack.back().stackBase;
        push(value);
        callStack.pop_back();
        DISPATCH();
    }



    L_IF_FALSE: {
#if DEBUG
        opcodeCount[0x1A]++;
#endif
        uint32_t offset = read32();
        if (pop() == 0) {
            ip += offset;
        }

        DISPATCH();
    }


    L_JMP: {
#if DEBUG
        opcodeCount[0x1A]++;
#endif
        int32_t offset = readSigned32();
        ip += offset;

        DISPATCH();
    }

    L_NEG : {
#if DEBUG
        opcodeCount[0x1C]++;
#endif
        uint8_t type = read();
        negTable[type](*this);
        DISPATCH();
    }


    L_INC : {
#if DEBUG
        opcodeCount[0x1D]++;
#endif
        uint8_t callFrame = read();
        uint8_t scope = read();
        uint32_t varId = read32();

        CallFrame* targetFrame = &callStack[callStack.size() - callFrame - 1];

        uint64_t targetScope = targetFrame->scopeSp[targetFrame->scopeSp.size() - scope - 1];

        push(++stackBuf[targetScope + varId]);
        DISPATCH();
    }

    L_DEC : {
#if DEBUG
        opcodeCount[0x1E]++;
#endif
        uint8_t callFrame = read();
        uint8_t scope = read();
        uint32_t varId = read32();

        CallFrame* targetFrame = &callStack[callStack.size() - callFrame - 1];

        uint64_t targetScope = targetFrame->scopeSp[targetFrame->scopeSp.size() - scope - 1];

        push(--stackBuf[targetScope + varId]);
        DISPATCH();
    }



    L_POST_INC : {
#if DEBUG
        opcodeCount[0x1F]++;
#endif
        uint8_t callFrame = read();
        uint8_t scope = read();
        uint32_t varId = read32();

        CallFrame* targetFrame = &callStack[callStack.size() - callFrame - 1];

        uint64_t targetScope = targetFrame->scopeSp[targetFrame->scopeSp.size() - scope - 1];

        push(stackBuf[targetScope + varId]++);

        DISPATCH();
    }


    L_POST_DEC : {
#if DEBUG
        opcodeCount[0x20]++;
#endif
        uint8_t callFrame = read();
        uint8_t scope = read();
        uint32_t varId = read32();

        CallFrame* targetFrame = &callStack[callStack.size() - callFrame - 1];

        uint64_t targetScope = targetFrame->scopeSp[targetFrame->scopeSp.size() - scope - 1];

        push(stackBuf[targetScope + varId]--);
        DISPATCH();
    }




    L_GREATER_EQUAL: {
#if DEBUG
        opcodeCount[0x21]++;
#endif
        uint8_t type = read();
        geTable[type](*this);
        DISPATCH();
    }

    L_LESS_EQUAL: {
#if DEBUG
        opcodeCount[0x22]++;
#endif
        uint8_t type = read();
        leTable[type](*this);
        DISPATCH();
    }


    L_NOT_EQUAL: {
#if DEBUG
        opcodeCount[0x23]++;
#endif
        uint8_t type = read();
        neTable[type](*this);
        DISPATCH();
    }



    L_AND : {
#if DEBUG
        opcodeCount[0x24]++;
#endif
        uint8_t type = read();
        andTable[type](*this);
        DISPATCH();
    }


    L_OR : {
#if DEBUG
        opcodeCount[0x25]++;
#endif
        uint8_t type = read();
        orTable[type](*this);
        DISPATCH();
    }




    L_NOT : {
#if DEBUG
        opcodeCount[0x26]++;
#endif
        uint8_t type = read();
        notTable[type](*this);
        DISPATCH();
    }


    L_BIT_AND : {
#if DEBUG
        opcodeCount[0x27]++;
#endif
        uint8_t type = read();
        bitAndTable[type](*this);
        DISPATCH();
    }


    L_BIT_OR : {
#if DEBUG
        opcodeCount[0x28]++;
#endif
        uint8_t type = read();
        bitOrTable[type](*this);
        DISPATCH();
    }



    L_BIT_XOR : {
#if DEBUG
        opcodeCount[0x29]++;
#endif
        uint8_t type = read();
        bitXorTable[type](*this);
        DISPATCH();
    }


    L_SHIFT_LEFT : {
#if DEBUG
        opcodeCount[0x2A]++;
#endif
        uint8_t type = read();
        shiftLeftTable[type](*this);
        DISPATCH();
    }

    L_SHIFT_RIGHT : {
#if DEBUG
        opcodeCount[0x2B]++;
#endif
        uint8_t type = read();
        shiftRightTable[type](*this);
        DISPATCH();
    }

    L_BIT_NOT : {
#if DEBUG
        opcodeCount[0x2C]++;
#endif
        uint8_t type = read();
        bitNotTable[type](*this);
        DISPATCH();
    }


    L_HALT: {
    #if DEBUG
        opcodeCount[0x00]++;
        std::cout << "\n\n\n-------LOGGING-------" << std::endl;
        std::fflush(stdout);
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
