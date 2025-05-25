//
// Created by home on 25/05/25.
//

#ifndef BIT_SR_HANDLERS_H
#define BIT_SR_HANDLERS_H


#include "orion_vm.h"

using BitShiftRightFn = void(*)(OrionVM&);

static inline void bit_shr_i32(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(value) >> static_cast<int32_t>(shift)));
}

static inline void bit_shr_i64(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(value) >> static_cast<int64_t>(shift)));
}

static inline void bit_shr_i8(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(value) >> static_cast<int8_t>(shift)));
}

static inline void bit_shr_i16(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(value) >> static_cast<int16_t>(shift)));
}

static inline void bit_shr_char(OrionVM& vm) {
    RawValue shift = vm.popValue();
    RawValue value = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(value) >> static_cast<uint32_t>(shift));
}

static constexpr BitShiftRightFn shiftRightTable[MAX_TYPE] = {
    nullptr, bit_shr_i32, bit_shr_i64, nullptr, nullptr, nullptr, nullptr,
    bit_shr_i8, bit_shr_i16, bit_shr_char
};


#endif //BIT_SR_HANDLERS_H
