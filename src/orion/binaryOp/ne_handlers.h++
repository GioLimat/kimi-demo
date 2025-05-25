//
// Created by home on 24/05/25.
//

#ifndef NE_HANDLERS_H
#define NE_HANDLERS_H


// ne_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using NeFn = void(*)(OrionVM&);

static inline void ne_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) != static_cast<int32_t>(b)));
}

static inline void ne_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) != static_cast<int64_t>(b)));
}

static inline void ne_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    vm.pushValue(static_cast<RawValue>(fa != fb));
}

static inline void ne_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    vm.pushValue(da != db);
}

static inline void ne_bool(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(a != 0 != (b != 0));
}

static inline void ne_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int8_t>(a) != static_cast<int8_t>(b));
}

static inline void ne_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<int16_t>(a) != static_cast<int16_t>(b));
}

static inline void ne_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) != static_cast<uint32_t>(b));
}

static constexpr NeFn neTable[256] = {
    nullptr, ne_i32, ne_i64, ne_f32, ne_f64, ne_bool, nullptr,
    ne_i8,   ne_i16, ne_char
};


#endif //NE_HANDLERS_H
