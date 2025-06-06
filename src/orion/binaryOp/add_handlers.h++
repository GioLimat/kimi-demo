//
// Created by home on 24/05/25.
//

#ifndef ADD_HANDLERS_H
#define ADD_HANDLERS_H

#include "orion_vm.h"
#include "string_header.h++"
#include <cstring>

using AddFn = void(*)(OrionVM&);

static inline void add_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) + static_cast<int32_t>(b)));
}

static inline void add_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) + static_cast<int64_t>(b)));
}

static inline void add_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    float fr = fa + fb;
    RawValue bits;
    std::memcpy(&bits, &fr, sizeof(fr));
    vm.pushValue(bits);
}

static inline void add_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    double dr = da + db;
    RawValue bits;
    std::memcpy(&bits, &dr, sizeof(dr));
    vm.pushValue(bits);
}

static inline void add_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) + static_cast<int8_t>(b)));
}

static inline void add_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) + static_cast<int16_t>(b)));
}

static inline void add_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<uint32_t>(a) + static_cast<uint32_t>(b)));
}


static inline void add_str(OrionVM& vm) {
    RawValue bAddr = vm.popValue(); // RHS
    RawValue aAddr = vm.popValue(); // LHS

    auto* aRaw = vm.heapPtrFromAddr(aAddr);
    auto* bRaw = vm.heapPtrFromAddr(bAddr);

    auto* aHdr = reinterpret_cast<const StringHeader*>(aRaw);
    auto* bHdr = reinterpret_cast<const StringHeader*>(bRaw);

    auto aLen = aHdr->length;
    auto bLen = bHdr->length;



    const char* aPayload = reinterpret_cast<const char*>(aRaw + sizeof(StringHeader));
    const char* bPayload = reinterpret_cast<const char*>(bRaw + sizeof(StringHeader));

    uint64_t totalLen = aLen + bLen;
    size_t totalSize = sizeof(StringHeader) + totalLen;

    uint8_t* newBlock = vm.alloc(totalSize);

    auto* newHdr = reinterpret_cast<StringHeader*>(newBlock);
    newHdr->length = totalLen;

    char* newPayload = reinterpret_cast<char*>(newBlock + sizeof(StringHeader));
    std::memcpy(newPayload, aPayload, aLen);
    std::memcpy(newPayload + aLen, bPayload, bLen);

    RawValue result = vm.addrFromHeapPtr(newBlock);
    vm.pushValue(result);
}



static constexpr AddFn addTable[256] = {
    nullptr,  // 0x00
    add_i32,  // 0x01
    add_i64,  // 0x02
    add_f32,  // 0x03
    add_f64,  // 0x04
    nullptr,  // 0x05 (bool)
    nullptr,  // 0x06
    add_i8,   // 0x07
    add_i16,  // 0x08
    add_char,  // 0x09
    add_str
};

#endif // ADD_HANDLERS_H
