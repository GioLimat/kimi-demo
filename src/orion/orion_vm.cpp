//
// Created by home on 22/05/25.
//

#include "orion_vm.h"

#include <cstring>
#include <iostream>

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
    OPCODE_CASE(0x13, L_GREATER);
    OPCODE_CASE(0x14, L_LESS);
    OPCODE_CASE(0x15, L_EQUAL);

    //OPCODE_CASE(0x1C, L_NEG);


    DISPATCH();




    L_CONST: {
    #if DEBUG
        opcodeCount[0x01]++;
    #endif
        read();
        uint8_t type = read();
        push(readPayload(type));
        DISPATCH();
    }






    L_LOAD: {
    #if DEBUG
        opcodeCount[0x02]++;
    #endif
        uint8_t meta = read();
        uint8_t type = read();
        uint32_t varId = read32();

        uint8_t callDepth = meta >> 4;
        uint8_t scope = meta & 0x0F;

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
        read();
        uint8_t type = read();
        uint32_t varId = read32();

        RawValue value = pop();
        stackBuf[callStack.back().stackBase + varId] = value;
        DISPATCH();
    }




     L_PRINT: {
    #if DEBUG
        opcodeCount[0x04]++;
    #endif
        uint8_t type = read();
        RawValue val = pop();
        switch (type) {
            case 0x01: // i32 (4 bytes)
                printf("%d\n", static_cast<int32_t>(val));
                break;
            case 0x02: // i64 (8 bytes)
                printf("%lld\n", static_cast<long long>(val));
                break;
            case 0x03: { // f32 (4 bytes)
                float f;
                std::memcpy(&f, &val, sizeof(f));
                printf("%f\n", f);
                break;
            }
            case 0x04: { // f64 (8 bytes)
                double d;
                std::memcpy(&d, &val, sizeof(d));
                printf("%lf\n", d);
                break;
            }
            case 0x05: // bool (1 byte)
                printf("%s\n", val ? "true" : "false");
                break;
            case 0x07: // i8 (1 byte)
                printf("%d\n", static_cast<int8_t>(val));
                break;
            case 0x08: // i16 (2 bytes)
                printf("%d\n", static_cast<int16_t>(val));
                break;
            case 0x09: { // char (4 bytes UTF-32)
                auto c = static_cast<uint32_t>(val);
                char utf8[5] = {0};
                if (c <= 0x7F) {
                    utf8[0] = static_cast<char>(c);
                } else if (c <= 0x7FF) {
                    utf8[0] = static_cast<char>(0xC0 | ((c >> 6) & 0x1F));
                    utf8[1] = static_cast<char>(0x80 | (c & 0x3F));
                } else if (c <= 0xFFFF) {
                    utf8[0] = static_cast<char>(0xE0 | ((c >> 12) & 0x0F));
                    utf8[1] = static_cast<char>(0x80 | ((c >> 6) & 0x3F));
                    utf8[2] = static_cast<char>(0x80 | (c & 0x3F));
                } else {
                    utf8[0] = static_cast<char>(0xF0 | ((c >> 18) & 0x07));
                    utf8[1] = static_cast<char>(0x80 | ((c >> 12) & 0x3F));
                    utf8[2] = static_cast<char>(0x80 | ((c >> 6) & 0x3F));
                    utf8[3] = static_cast<char>(0x80 | (c & 0x3F));
                }
                printf("%s\n", utf8);
                break;
            }
            default:
                printf("Unknown type\n");
                break;
        }
        DISPATCH();
    }






    L_ADD: {
    #if DEBUG
        opcodeCount[0x0E]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: // i32 (4 bytes)
                push(static_cast<RawValue>(static_cast<int32_t>(a) + static_cast<int32_t>(b)));
                break;
            case 0x02: // i64 (8 bytes)
                push(static_cast<RawValue>(static_cast<int64_t>(a) + static_cast<int64_t>(b)));
                break;
            case 0x03: { // f32 (4 bytes)
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                float fr = fa + fb;
                uint64_t bits;
                std::memcpy(&bits, &fr, sizeof(fr));
                push(bits);
                break;
            }
            case 0x04: { // f64 (8 bytes)
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                double dr = da + db;
                uint64_t bits;
                std::memcpy(&bits, &dr, sizeof(dr));
                push(bits);
                break;
            }
            case 0x07: // i8 (1 byte)
                push(static_cast<RawValue>(static_cast<int8_t>(a) + static_cast<int8_t>(b)));
                break;
            case 0x08: // i16 (2 bytes)
                push(static_cast<RawValue>(static_cast<int16_t>(a) + static_cast<int16_t>(b)));
                break;
            case 0x09: // char (4 bytes UTF-32)
                push(static_cast<uint32_t>(a) + static_cast<uint32_t>(b));
                break;
        }
        DISPATCH();
    }






    L_SUB: {
    #if DEBUG
        opcodeCount[0x0F]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: // i32 (4 bytes)
                push(static_cast<RawValue>(static_cast<int32_t>(a) - static_cast<int32_t>(b)));
                break;
            case 0x02: // i64 (8 bytes)
                push(static_cast<RawValue>(static_cast<int64_t>(a) - static_cast<int64_t>(b)));
                break;
            case 0x03: { // f32 (4 bytes)
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                float fr = fa - fb;
                uint64_t bits;
                std::memcpy(&bits, &fr, sizeof(fr));
                push(bits);
                break;
            }
            case 0x04: { // f64 (8 bytes)
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                double dr = da - db;
                uint64_t bits;
                std::memcpy(&bits, &dr, sizeof(dr));
                push(bits);
                break;
            }
            case 0x07: // i8 (1 byte)
                push(static_cast<RawValue>(static_cast<int8_t>(a) - static_cast<int8_t>(b)));
                break;
            case 0x08: // i16 (2 bytes)
                push(static_cast<RawValue>(static_cast<int16_t>(a) - static_cast<int16_t>(b)));
                break;
            case 0x09: // char (4 bytes UTF-32)
                push(static_cast<uint32_t>(a) - static_cast<uint32_t>(b));
                break;
        }
        DISPATCH();
    }






    L_MUL: {
    #if DEBUG
    opcodeCount[0x10]++;
    #endif
    uint8_t t = read();
    read();
    RawValue b = pop();
    RawValue a = pop();
    switch (t) {
        case 0x01: // i32 (4 bytes)
            push(static_cast<RawValue>(static_cast<int32_t>(a) * static_cast<int32_t>(b)));
            break;
        case 0x02: // i64 (8 bytes)
            push(static_cast<RawValue>(static_cast<int64_t>(a) * static_cast<int64_t>(b)));
            break;
        case 0x03: { // f32 (4 bytes)
            float fa, fb;
            std::memcpy(&fa, &a, sizeof(fa));
            std::memcpy(&fb, &b, sizeof(fb));
            float fr = fa * fb;
            uint64_t bits;
            std::memcpy(&bits, &fr, sizeof(fr));
            push(bits);
            break;
        }
        case 0x04: { // f64 (8 bytes)
            double da, db;
            std::memcpy(&da, &a, sizeof(da));
            std::memcpy(&db, &b, sizeof(db));
            double dr = da * db;
            uint64_t bits;
            std::memcpy(&bits, &dr, sizeof(dr));
            push(bits);
            break;
        }
        case 0x07: // i8 (1 byte)
            push(static_cast<RawValue>(static_cast<int8_t>(a) * static_cast<int8_t>(b)));
            break;
        case 0x08: // i16 (2 bytes)
            push(static_cast<RawValue>(static_cast<int16_t>(a) * static_cast<int16_t>(b)));
            break;
        case 0x09: // char (4 bytes UTF-32)
            push(static_cast<uint32_t>(a) * static_cast<uint32_t>(b));
            break;
        }
        DISPATCH();
    }






    L_DIV: {
        #if DEBUG
        opcodeCount[0x11]++;
        #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: // i32 (4 bytes)
                push(static_cast<RawValue>(static_cast<int32_t>(a) / static_cast<int32_t>(b)));
                break;
            case 0x02: // i64 (8 bytes)
                push(static_cast<RawValue>(static_cast<int64_t>(a) / static_cast<int64_t>(b)));
                break;
            case 0x03: { // f32 (4 bytes)
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                float fr = fa / fb;
                uint64_t bits;
                std::memcpy(&bits, &fr, sizeof(fr));
                push(bits);
                break;
            }
            case 0x04: { // f64 (8 bytes)
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                double dr = da / db;
                uint64_t bits;
                std::memcpy(&bits, &dr, sizeof(dr));
                push(bits);
                break;
            }
            case 0x07: // i8 (1 byte)
                push(static_cast<RawValue>(static_cast<int8_t>(a) / static_cast<int8_t>(b)));
                break;
            case 0x08: // i16 (2 bytes)
                push(static_cast<RawValue>(static_cast<int16_t>(a) / static_cast<int16_t>(b)));
                break;
            case 0x09: // char (4 bytes UTF-32)
                push(static_cast<uint32_t>(a) / static_cast<uint32_t>(b));
                break;
        }
        DISPATCH();
    }







    L_MOD: {
    #if DEBUG
        opcodeCount[0x12]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: // i32 (4 bytes)
                push(static_cast<RawValue>(static_cast<int32_t>(a) % static_cast<int32_t>(b)));
                break;
            case 0x02: // i64 (8 bytes)
                push(static_cast<RawValue>(static_cast<int64_t>(a) % static_cast<int64_t>(b)));
                break;
            case 0x07: // i8 (1 byte)
                push(static_cast<RawValue>(static_cast<int8_t>(a) % static_cast<int8_t>(b)));
                break;
            case 0x08: // i16 (2 bytes)
                push(static_cast<RawValue>(static_cast<int16_t>(a) % static_cast<int16_t>(b)));
                break;
            case 0x09: // char (4 bytes UTF-32)
                push(static_cast<uint32_t>(a) % static_cast<uint32_t>(b));
                break;
        }
        DISPATCH();
    }






    L_GREATER: {
    #if DEBUG
        opcodeCount[0x13]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: push(static_cast<int32_t>(a) > static_cast<int32_t>(b)); break; // i32
            case 0x02: push(static_cast<int64_t>(a) > static_cast<int64_t>(b)); break; // i64
            case 0x03: {
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                push(fa > fb);
                break;
            }
            case 0x04: {
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                push(da > db);
                break;
            }
            case 0x07: push(static_cast<int8_t>(a) > static_cast<int8_t>(b)); break;  // i8
            case 0x08: push(static_cast<int16_t>(a) > static_cast<int16_t>(b)); break; // i16
            case 0x09: push(static_cast<uint32_t>(a) > static_cast<uint32_t>(b)); break; // char (UTF-32)
        }
        DISPATCH();
    }





    L_LESS: {
    #if DEBUG
        opcodeCount[0x14]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: push(static_cast<int32_t>(a) < static_cast<int32_t>(b)); break; // i32
            case 0x02: push(static_cast<int64_t>(a) < static_cast<int64_t>(b)); break; // i64
            case 0x03: {
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                push(fa < fb);
                break;
            }
            case 0x04: {
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                push(da < db);
                break;
            }
            case 0x07: push(static_cast<int8_t>(a) < static_cast<int8_t>(b)); break;  // i8
            case 0x08: push(static_cast<int16_t>(a) < static_cast<int16_t>(b)); break; // i16
            case 0x09: push(static_cast<uint32_t>(a) < static_cast<uint32_t>(b)); break; // char (UTF-32)
        }
        DISPATCH();
    }
    L_EQUAL: {
    #if DEBUG
        opcodeCount[0x15]++;
    #endif
        uint8_t t = read();
        read();
        RawValue b = pop();
        RawValue a = pop();
        switch (t) {
            case 0x01: push(static_cast<int32_t>(a) == static_cast<int32_t>(b)); break; // i32
            case 0x02: push(static_cast<int64_t>(a) == static_cast<int64_t>(b)); break; // i64
            case 0x03: {
                float fa, fb;
                std::memcpy(&fa, &a, sizeof(fa));
                std::memcpy(&fb, &b, sizeof(fb));
                push(fa == fb);
                break;
            }
            case 0x04: {
                double da, db;
                std::memcpy(&da, &a, sizeof(da));
                std::memcpy(&db, &b, sizeof(db));
                push(da == db);
                break;
            }
            case 0x07: push(static_cast<int8_t>(a) == static_cast<int8_t>(b)); break;  // i8
            case 0x08: push(static_cast<int16_t>(a) == static_cast<int16_t>(b)); break; // i16
            case 0x09: push(static_cast<uint32_t>(a) == static_cast<uint32_t>(b)); break; // char (UTF-32)
        }
        DISPATCH();
    }







    L_HALT: {
    #if DEBUG
        opcodeCount[0x00]++;
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
