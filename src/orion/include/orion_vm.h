//
// Created by home on 22/05/25.
//

#ifndef ORION_VM_H
#define ORION_VM_H

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>


#define MAX_TYPE 0xFF

static constexpr uint64_t HEAP_BASE = 0x2000000000000000ULL;

static constexpr size_t HEAP_CAPACITY = 16 * 1024 * 1024;

static constexpr size_t STACK_MAX = 65536;


#define DEBUG 1
using RawValue = uint64_t;

struct FunctionInfo {
    uint64_t initIp;
    uint64_t endIp;
    uint64_t argsSize;
};

struct CallFrame {
    uint64_t    returnIp;
    uint64_t    stackBase;
    CallFrame*  parent;
    std::vector<uint64_t> scopeSp;
};

class OrionVM {
public:
    explicit OrionVM(const std::vector<uint8_t>& bytecode);
    explicit OrionVM() = default;

    std::vector<uint8_t> bytecode;
    void run();

    RawValue popValue() {
        return stackBuf[--sp];
    }
    void pushValue(const RawValue value) {
        stackBuf[sp++] = value;
    }

    void preprocessFunctions();

    inline uint8_t* heapPtrFromAddr(uint64_t addr) {
        auto offset = static_cast<size_t>(addr - HEAP_BASE);
        return heapBuffer.data() + offset;
    }

    uint64_t heapAllocate(size_t rawSize) {

        size_t aligned = (rawSize + 7) & ~static_cast<size_t>(7);

        if (heapOffset + aligned > HEAP_CAPACITY) {
            std::fprintf(stderr, "VM out of heap memory (requested %zu bytes)\n", rawSize);
            std::abort();
        }
        uint64_t addr = HEAP_BASE + static_cast<uint64_t>(heapOffset);
        heapOffset += aligned;
        if (heapBuffer.size() < heapOffset) {
            heapBuffer.resize(heapOffset);
        }
        return addr;
    }

private:
    /*READER
   *
   */
    using ReaderFn = RawValue(*)(OrionVM*);

    static RawValue read0(OrionVM*) { return 0; }
    static RawValue readI32(OrionVM* vm) {
        uint32_t r = 0;
        for (int i = 0; i < 4; ++i) r |= static_cast<uint32_t>(vm->read()) << (8 * i);
        return static_cast<RawValue>(static_cast<int32_t>(r));
    }
    static RawValue readI64(OrionVM* vm) {
        uint64_t r = 0;
        for (int i = 0; i < 8; ++i) r |= static_cast<uint64_t>(vm->read()) << (8 * i);
        return r;
    }
    static RawValue readF32(OrionVM* vm) {
        uint32_t raw = 0;
        for (int i = 0; i < 4; ++i) raw |= static_cast<uint32_t>(vm->read()) << (8 * i);
        float f; std::memcpy(&f, &raw, 4);
        uint64_t bits; std::memcpy(&bits, &f, 4);
        return bits;
    }
    static RawValue readF64(OrionVM* vm) {
        uint64_t r = 0;
        for (int i = 0; i < 8; ++i) r |= static_cast<uint64_t>(vm->read()) << (8 * i);
        return r;
    }
    static RawValue readBool(OrionVM* vm) {
        return vm->read() != 0;
    }
    static RawValue readI8(OrionVM* vm) {
        return static_cast<RawValue>(static_cast<int8_t>(vm->read()));
    }
    static RawValue readI16(OrionVM* vm) {
        uint16_t r = 0;
        for (int i = 0; i < 2; ++i) r |= static_cast<uint16_t>(vm->read()) << (8 * i);
        return static_cast<RawValue>(static_cast<int16_t>(r));
    }
    static RawValue readChar(OrionVM* vm) {
        uint32_t r = 0;
        for (int i = 0; i < 4; ++i) r |= static_cast<uint32_t>(vm->read()) << (8 * i);
        return r;
    }

    static RawValue readStrSmall(OrionVM* vm) {
        uint64_t r = 0;
        for (int i = 0; i < 8; ++i) r |= static_cast<uint64_t>(vm->read()) << (8 * i);
        return r;
    }

    static RawValue readHeapAddr(OrionVM* vm) {
        uint64_t addr = 0;
        for (int i = 0; i < 8; ++i) {
            addr |= static_cast<uint64_t>(vm->read()) << (8 * i);
        }
        return addr;
    }

    static constexpr ReaderFn payloadReaders[256] = {
        /* 0x00 */ read0,
        /* 0x01 */ readI32,
        /* 0x02 */ readI64,
        /* 0x03 */ readF32,
        /* 0x04 */ readF64,
        /* 0x05 */ readBool,
        /* 0x06 */ readStrSmall,
        /* 0x07 */ readI8,
        /* 0x08 */ readI16,
        /* 0x09 */ readChar,
        /* 0x0A */ readHeapAddr,
    };

    RawValue readPayload(uint8_t type) {
        return payloadReaders[type](this);
    }

    /*READER
     *
     */


    std::vector<FunctionInfo> functions;
    uint64_t                   ip    = 0;
    std::vector<CallFrame>     callStack;
    RawValue                   regA  = 0;
    RawValue                   regB  = 0;
    std::vector<uint8_t> heapBuffer;
    uint64_t heapOffset = 0;

    RawValue stackBuf[STACK_MAX]{};
    size_t sp = 0;



    inline void push(const RawValue v) {
        stackBuf[sp++] = v;
    }

    inline RawValue pop() {
        return stackBuf[--sp];
    }

    inline RawValue& local(size_t index) {
        return stackBuf[callStack.back().stackBase + index];
    }


#if DEBUG
    static constexpr size_t MAX_OPCODE = 0x2C + 1;
    uint64_t opcodeCount[MAX_OPCODE] = {0};
#endif

    uint8_t read() { return bytecode[ip++]; }

    inline uint32_t read32() {
        uint32_t id = 0;
        for (int i = 0; i < 4; ++i) id |= static_cast<uint32_t>(read()) << (8 * i);
        return id;
    }

    inline int32_t readSigned32() {
        int32_t id = 0;
        for (int i = 0; i < 4; ++i) id |= static_cast<int32_t>(read()) << (8 * i);
        return id;
    }



    void registerFunction(uint64_t* returnPos);
    uint64_t getInstructionLength(uint8_t op);

    static uint64_t decodeType(uint8_t type);
};

#endif // ORION_VM_H
