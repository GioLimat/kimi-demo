//
// Created by home on 22/05/25.
//

#ifndef ORION_VM_H
#define ORION_VM_H

#include <cstdint>
#include <vector>

static constexpr size_t STACK_MAX = 65536;

#define DEBUG 1
using RawValue = uint64_t;

struct CallFrame {
    uint64_t    returnIp;
    uint64_t    stackBase;
    CallFrame*  parent;
};

class OrionVM {
public:
    explicit OrionVM(const std::vector<uint8_t>& bytecode);
    void run();



private:
    const std::vector<uint8_t> bytecode;
    uint64_t                   ip    = 0;
    std::vector<CallFrame>     callStack;
    RawValue                   regA  = 0;
    RawValue                   regB  = 0;

    RawValue stackBuf[STACK_MAX];
    size_t sp = 0;

    inline void push(RawValue v) {
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
    RawValue readPayload(uint8_t type);
};

#endif // ORION_VM_H
