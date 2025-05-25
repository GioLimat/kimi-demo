//
// Created by home on 24/05/25.
//

#ifndef GE_HANDLERS_H
#define GE_HANDLERS_H

// ge_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using GeFn = void(*)(OrionVM&);

static inline void ge_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) >= static_cast<int32_t>(b));
}

static inline void ge_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) >= static_cast<int64_t>(b));
}

static inline void ge_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(fa >= fb);
}

static inline void ge_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da >= db);
}

static inline void ge_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) >= static_cast<int8_t>(b));
}

static inline void ge_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) >= static_cast<int16_t>(b));
}

static inline void ge_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) >= static_cast<uint32_t>(b));
}

static constexpr GeFn geTable[256] = {
    nullptr, ge_i32, ge_i64, ge_f32, ge_f64, nullptr, nullptr,
    ge_i8, ge_i16, ge_char
};


#endif //GE_HANDLERS_H
