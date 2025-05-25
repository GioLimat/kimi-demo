//
// Created by home on 25/05/25.
//

#ifndef BIT_OR_HANDLERS_H
#define BIT_OR_HANDLERS_H


#include "orion_vm.h"
#include <cstring>

using BitOrFn = void(*)(OrionVM&);

static inline void bit_or_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) | static_cast<int32_t>(b)));
}

static inline void bit_or_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) | static_cast<int64_t>(b)));
}

static inline void bit_or_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) | static_cast<int8_t>(b)));
}

static inline void bit_or_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) | static_cast<int16_t>(b)));
}

static inline void bit_or_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

static constexpr BitOrFn bitOrTable[MAX_TYPE] = {
    nullptr, bit_or_i32, bit_or_i64, nullptr, nullptr, nullptr, nullptr,
    bit_or_i8, bit_or_i16, bit_or_char
};


#endif //BIT_OR_HANDLERS_H
