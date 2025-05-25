//
// Created by home on 25/05/25.
//

#ifndef BIT_XOR_HANDLERS_H
#define BIT_XOR_HANDLERS_H

#include "orion_vm.h"

using BitXorFn = void(*)(OrionVM&);

static inline void bit_xor_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) ^ static_cast<int32_t>(b)));
}

static inline void bit_xor_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) ^ static_cast<int64_t>(b)));
}

static inline void bit_xor_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) ^ static_cast<int8_t>(b)));
}

static inline void bit_xor_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) ^ static_cast<int16_t>(b)));
}

static inline void bit_xor_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
}

static constexpr BitXorFn bitXorTable[MAX_TYPE] = {
    nullptr, bit_xor_i32, bit_xor_i64, nullptr, nullptr, nullptr, nullptr,
    bit_xor_i8, bit_xor_i16, bit_xor_char
};


#endif //BIT_XOR_HANDLERS_H
