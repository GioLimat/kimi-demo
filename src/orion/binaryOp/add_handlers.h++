//
// Created by home on 24/05/25.
//

#ifndef ADD_HANDLERS_H
#define ADD_HANDLERS_H


#include "orion_vm.h"
#include <cstring>

using AddFn = void(*)(OrionVM&);

static inline void add_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) + static_cast<int32_t>(b)));
}

static inline void add_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) + static_cast<int64_t>(b)));
}

static inline void add_f32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    float fa, fb;
    std::memcpy(&fa, &a, sizeof(fa));
    std::memcpy(&fb, &b, sizeof(fb));
    float fr = fa + fb;
    RawValue bits;
    std::memcpy(&bits, &fr, sizeof(fr));
    vm.pushValue(bits);
}

static inline void add_f64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    double da, db;
    std::memcpy(&da, &a, sizeof(da));
    std::memcpy(&db, &b, sizeof(db));
    double dr = da + db;
    RawValue bits;
    std::memcpy(&bits, &dr, sizeof(dr));

    vm.pushValue(bits);
}

static inline void add_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) + static_cast<int8_t>(b)));
}

static inline void add_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) + static_cast<int16_t>(b)));
}

static inline void add_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) + static_cast<uint32_t>(b));
}

static constexpr AddFn addTable[256] = {
    nullptr, add_i32, add_i64, add_f32, add_f64, nullptr, nullptr,
    add_i8, add_i16, add_char
};


#endif //ADD_HANDLERS_H
