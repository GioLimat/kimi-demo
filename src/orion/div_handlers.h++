//
// Created by home on 24/05/25.
//

#ifndef DIV_HANDLERS_H
#define DIV_HANDLERS_H


// div_handlers.hpp
#pragma once
#include "orion_vm.h"
#include <cstring>

using DivFn = void(*)(OrionVM&);

static inline void div_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) / static_cast<int32_t>(b)));
}

static inline void div_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) / static_cast<int64_t>(b)));
}

static inline void div_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    float fr = fa / fb;
    RawValue bits;
    std::memcpy(&bits, &fr, sizeof(fr));
    vm.pushValue(bits);
}

static inline void div_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    double dr = da / db;
    RawValue bits;
    std::memcpy(&bits, &dr, sizeof(dr));
    vm.pushValue(bits);
}

static inline void div_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) / static_cast<int8_t>(b)));
}

static inline void div_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) / static_cast<int16_t>(b)));
}

static inline void div_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) / static_cast<uint32_t>(b));
}

static constexpr DivFn divTable[256] = {
    nullptr, div_i32, div_i64, div_f32, div_f64, nullptr, nullptr,
    div_i8, div_i16, div_char
};


#endif //DIV_HANDLERS_H
