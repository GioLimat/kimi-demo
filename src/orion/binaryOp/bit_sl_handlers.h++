//
// Created by home on 25/05/25.
//

#ifndef BIT_SL_HANDLERS_H
#define BIT_SL_HANDLERS_H

#include "orion_vm.h"

using ShiftLeftFn = void(*)(OrionVM&);

static inline void shift_left_i32(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(value) << static_cast<int32_t>(shift)));
}

static inline void shift_left_i64(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(value) << static_cast<int64_t>(shift)));
}

static inline void shift_left_i8(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(value) << static_cast<int8_t>(shift)));
}

static inline void shift_left_i16(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(value) << static_cast<int16_t>(shift)));
}

static inline void shift_left_char(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(value) << static_cast<uint32_t>(shift));
}

static constexpr ShiftLeftFn shiftLeftTable[256] = {
    nullptr, shift_left_i32, shift_left_i64, nullptr, nullptr, nullptr, nullptr,
    shift_left_i8, shift_left_i16, shift_left_char
};

#endif //BIT_SL_HANDLERS_H
