//
// Created by home on 24/05/25.
//

#ifndef MOD_HANDLERS_H
#define MOD_HANDLERS_H


// mod_handlers.hpp
#pragma once
#include "orion_vm.h"

using ModFn = void(*)(OrionVM&);

static inline void mod_i32(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int32_t>(a) % static_cast<int32_t>(b)));
}

static inline void mod_i64(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int64_t>(a) % static_cast<int64_t>(b)));
}

static inline void mod_i8(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int8_t>(a) % static_cast<int8_t>(b)));
}

static inline void mod_i16(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<RawValue>(static_cast<int16_t>(a) % static_cast<int16_t>(b)));
}

static inline void mod_char(OrionVM& vm) {
    RawValue b = vm.popValue(), a = vm.popValue();
    vm.pushValue(static_cast<uint32_t>(a) % static_cast<uint32_t>(b));
}

static constexpr ModFn modTable[256] = {
    nullptr, mod_i32, mod_i64, nullptr, nullptr, nullptr, nullptr,
    mod_i8,  mod_i16, mod_char
};


#endif //MOD_HANDLERS_H
