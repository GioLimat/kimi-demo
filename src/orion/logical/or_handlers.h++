//
// Created by home on 25/05/25.
//

// or_handlers.h
#ifndef OR_HANDLERS_H
#define OR_HANDLERS_H

#include "orion_vm.h"
#include <cstring>

using OrFn = void(*)(OrionVM&);

static inline void or_i32(OrionVM& vm) {
    auto b = static_cast<int32_t>(vm.popValue());
    auto a = static_cast<int32_t>(vm.popValue());
    vm.pushValue(a || b);
}

static inline void or_i64(OrionVM& vm) {
    auto b = static_cast<int64_t>(vm.popValue());
    auto a = static_cast<int64_t>(vm.popValue());
    vm.pushValue(a || b);
}

static inline void or_f32(OrionVM& vm) {
    RawValue vb = vm.popValue(), va = vm.popValue();
    float fb, fa;
    std::memcpy(&fa, &va, sizeof(fa));
    std::memcpy(&fb, &vb, sizeof(fb));
    vm.pushValue(fa || fb);
}

static inline void or_f64(OrionVM& vm) {
    RawValue vb = vm.popValue(), va = vm.popValue();
    double db, da;
    std::memcpy(&da, &va, sizeof(da));
    std::memcpy(&db, &vb, sizeof(db));
    vm.pushValue(da || db);
}

static inline void or_bool(OrionVM& vm) {
    auto b = vm.popValue(), a = vm.popValue();
    vm.pushValue(a || b);
}

static inline void or_i8(OrionVM& vm) {
    auto b = static_cast<int8_t>(vm.popValue());
    auto a = static_cast<int8_t>(vm.popValue());
    vm.pushValue(a || b);
}

static inline void or_i16(OrionVM& vm) {
    auto b = static_cast<int16_t>(vm.popValue());
    auto a = static_cast<int16_t>(vm.popValue());
    vm.pushValue(a || b);
}

static inline void or_char(OrionVM& vm) {
    auto b = static_cast<uint32_t>(vm.popValue());
    auto a = static_cast<uint32_t>(vm.popValue());
    vm.pushValue(a || b);
}

static constexpr OrFn orTable[256] = {
    nullptr,
    or_i32,
    or_i64,
    or_f32,
    or_f64,
    or_bool,
    nullptr,
    or_i8,
    or_i16,
    or_char
};

#endif

