//
// Created by home on 24/05/25.
//

#ifndef LESS_HANDLERS_H
#define LESS_HANDLERS_H


// less_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using CmpFn = void(*)(OrionVM&);

static inline void less_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) < static_cast<int32_t>(b));
}

static inline void less_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) < static_cast<int64_t>(b));
}

static inline void less_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(fa < fb);
}

static inline void less_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da < db);
}

static inline void less_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) < static_cast<int8_t>(b));
}

static inline void less_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) < static_cast<int16_t>(b));
}

static inline void less_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) < static_cast<uint32_t>(b));
}

static constexpr CmpFn lessTable[256] = {
    nullptr, less_i32, less_i64, less_f32, less_f64, nullptr, nullptr,
    less_i8, less_i16, less_char
};


#endif //LESS_HANDLERS_H
