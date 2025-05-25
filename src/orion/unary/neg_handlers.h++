//
// Created by home on 25/05/25.
//

#ifndef NEG_HANDLERS_H
#define NEG_HANDLERS_H


#include "orion_vm.h"
#include <cstring>

using NegFn = void(*)(OrionVM&);

static inline void neg_i32(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(-static_cast<int32_t>(a)));
}

static inline void neg_i64(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(-static_cast<int64_t>(a)));
}

static inline void neg_f32(OrionVM& vm) {
    RawValue a = vm.popValue();
    float fa;
    std::memcpy(&fa, &a, sizeof(fa));
    float fr = -fa;
    RawValue bits;
    std::memcpy(&bits, &fr, sizeof(fr));
    vm.pushValue(bits);
}

static inline void neg_f64(OrionVM& vm) {
    RawValue a = vm.popValue();
    double da;
    std::memcpy(&da, &a, sizeof(da));
    double dr = -da;
    RawValue bits;
    std::memcpy(&bits, &dr, sizeof(dr));
    vm.pushValue(bits);
}

static inline void neg_i8(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(-static_cast<int8_t>(a)));
}

static inline void neg_i16(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(-static_cast<int16_t>(a)));
}

static inline void neg_char(OrionVM& vm) {
    RawValue a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(-static_cast<int32_t>(a)));
}

static constexpr NegFn negTable[256] = {
    nullptr,
    neg_i32,
    neg_i64,
    neg_f32,
    neg_f64,
    nullptr,
    nullptr,
    neg_i8,
    neg_i16,
    neg_char
};


#endif //NEG_HANDLERS_H
