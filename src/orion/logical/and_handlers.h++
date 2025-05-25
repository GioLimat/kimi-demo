//
// Created by home on 25/05/25.
//

// and_handlers.h
#ifndef AND_HANDLERS_H
#define AND_HANDLERS_H

#include "orion_vm.h"
#include <cstring>

using AndFn = void(*)(OrionVM&);

static inline void and_i32(OrionVM& vm) {
    auto b = static_cast<int32_t>(vm.popValue());
    auto a = static_cast<int32_t>(vm.popValue());
    vm.pushValue(a && b);
}

static inline void and_i64(OrionVM& vm) {
    auto b = static_cast<int64_t>(vm.popValue());
    auto a = static_cast<int64_t>(vm.popValue());
    vm.pushValue(a && b);
}

static inline void and_f32(OrionVM& vm) {
    RawValue vb = vm.popValue(), va = vm.popValue();
    float fb, fa;
    std::memcpy(&fa, &va, sizeof(fa));
    std::memcpy(&fb, &vb, sizeof(fb));
    vm.pushValue(fa && fb);
}

static inline void and_f64(OrionVM& vm) {
    RawValue vb = vm.popValue(), va = vm.popValue();
    double db, da;
    std::memcpy(&da, &va, sizeof(da));
    std::memcpy(&db, &vb, sizeof(db));
    vm.pushValue(da && db);
}

static inline void and_bool(OrionVM& vm) {
    auto b = vm.popValue(), a = vm.popValue();
    vm.pushValue(a && b);
}

static inline void and_i8(OrionVM& vm) {
    auto b = static_cast<int8_t>(vm.popValue());
    auto a = static_cast<int8_t>(vm.popValue());
    vm.pushValue(a && b);
}

static inline void and_i16(OrionVM& vm) {
    auto b = static_cast<int16_t>(vm.popValue());
    auto a = static_cast<int16_t>(vm.popValue());
    vm.pushValue(a && b);
}

static inline void and_char(OrionVM& vm) {
    auto b = static_cast<uint32_t>(vm.popValue());
    auto a = static_cast<uint32_t>(vm.popValue());
    vm.pushValue(a && b);
}

static constexpr AndFn andTable[256] = {
    nullptr,
    and_i32,
    and_i64,
    and_f32,
    and_f64,
    and_bool,
    nullptr,
    and_i8,
    and_i16,
    and_char
};

#endif

