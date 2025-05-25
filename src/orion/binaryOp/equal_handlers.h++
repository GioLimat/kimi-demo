//
// Created by home on 24/05/25.
//

#ifndef EQUAL_HANDLERS_H
#define EQUAL_HANDLERS_H


// equal_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using EqFn = void(*)(OrionVM&);

static inline void eq_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) == static_cast<int32_t>(b));
}

static inline void eq_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) == static_cast<int64_t>(b));
}

static inline void eq_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(fa == fb);
}

static inline void eq_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da == db);
}

static inline void eq_bool(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(a != 0 == (b != 0));
}

static inline void eq_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) == static_cast<int8_t>(b));
}

static inline void eq_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) == static_cast<int16_t>(b));
}

static inline void eq_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) == static_cast<uint32_t>(b));
}

static constexpr EqFn equalTable[256] = {
    nullptr, eq_i32, eq_i64, eq_f32, eq_f64, eq_bool, nullptr,
    eq_i8, eq_i16, eq_char
};


#endif //EQUAL_HANDLERS_H
