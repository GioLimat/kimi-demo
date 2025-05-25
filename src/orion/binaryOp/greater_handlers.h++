//
// Created by home on 24/05/25.
//

#ifndef GREATER_HANDLERS_H
#define GREATER_HANDLERS_H


// greater_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using CmpFn = void(*)(OrionVM&);

static inline void greater_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int32_t>(a) > static_cast<int32_t>(b));
}

static inline void greater_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int64_t>(a) > static_cast<int64_t>(b));
}

static inline void greater_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(fa > fb);
}

static inline void greater_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da > db);
}

static inline void greater_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) > static_cast<int8_t>(b));
}

static inline void greater_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) > static_cast<int16_t>(b));
}

static inline void greater_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) > static_cast<uint32_t>(b));
}

static constexpr CmpFn greaterTable[256] = {
    nullptr, greater_i32, greater_i64, greater_f32, greater_f64, nullptr, nullptr,
    greater_i8, greater_i16, greater_char
};


#endif //GREATER_HANDLERS_H
