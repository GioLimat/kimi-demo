//
// Created by home on 25/05/25.
//

#ifndef NOT_HANDLERS_H
#define NOT_HANDLERS_H


// not_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using NotFn = void(*)(OrionVM&);

static inline void not_i32(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) == 0);
}

static inline void not_i64(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) == 0);
}

static inline void not_f32(OrionVM& vm) {
    RawValue a = vm.popValue();
    float fa; std::memcpy(&fa, &a, sizeof(fa));
    vm.pushValue(fa == 0.0f);
}

static inline void not_f64(OrionVM& vm) {
    RawValue a = vm.popValue();
    double da; std::memcpy(&da, &a, sizeof(da));
    vm.pushValue(da == 0.0);
}

static inline void not_bool(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(a == 0);
}

static inline void not_i8(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) == 0);
}

static inline void not_i16(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) == 0);
}

static inline void not_char(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) == 0);
}

static constexpr NotFn notTable[256] = {
    nullptr,
    not_i32,
    not_i64,
    not_f32,
    not_f64,
    not_bool,
    nullptr,
    not_i8,
    not_i16,
    not_char
};


#endif //NOT_HANDLERS_H
