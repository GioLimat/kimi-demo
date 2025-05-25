//
// Created by home on 25/05/25.
//

#ifndef BIT_NOT_HANDLERS_H
#define BIT_NOT_HANDLERS_H

#include "orion_vm.h"

using BitNotFn = void(*)(OrionVM&);

static inline void bit_not_i32(OrionVM& vm) {
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(~static_cast<int32_t>(value)));
}

static inline void bit_not_i64(OrionVM& vm) {
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(~static_cast<int64_t>(value)));
}

static inline void bit_not_i8(OrionVM& vm) {
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(~static_cast<int8_t>(value)));
}

static inline void bit_not_i16(OrionVM& vm) {
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(~static_cast<int16_t>(value)));
}

static inline void bit_not_char(OrionVM& vm) {
    RawValue value = vm.popValue();
    vm.pushValue(~static_cast<uint32_t>(value));
}

static constexpr BitNotFn bitNotTable[256] = {
    nullptr, bit_not_i32, bit_not_i64, nullptr, nullptr, nullptr, nullptr,
    bit_not_i8, bit_not_i16, bit_not_char
};


#endif //BIT_NOT_HANDLERS_H
