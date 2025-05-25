//
// Created by home on 24/05/25.
//

#ifndef LE_HANDLERS_H
#define LE_HANDLERS_H

// le_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using LeFn = void(*)(OrionVM&);

static inline void le_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) <= static_cast<int32_t>(b));
}

static inline void le_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) <= static_cast<int64_t>(b));
}

static inline void le_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(fa <= fb);
}

static inline void le_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da <= db);
}

static inline void le_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) <= static_cast<int8_t>(b));
}

static inline void le_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) <= static_cast<int16_t>(b));
}

static inline void le_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) <= static_cast<uint32_t>(b));
}

static constexpr LeFn leTable[256] = {
    nullptr, le_i32, le_i64, le_f32, le_f64, nullptr, nullptr,
    le_i8, le_i16, le_char
};


#endif //LE_HANDLERS_H
