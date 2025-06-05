//
// Created by home on 24/05/25.
//

#ifndef PRINT_HANDLERS_H
#define PRINT_HANDLERS_H



#include "orion_vm.h"
#include <cstdio>
#include <cstring>

#include "string_header.h++"
using PrintFn = void(*)(OrionVM* vm, RawValue);

static inline void print_str_small(OrionVM* /*vm*/, RawValue v) {
    char buf[9];
    for (int i = 0; i < 8; ++i) {
        buf[i] = static_cast<char>((v >> (8 * i)) & 0xFF);
    }
    buf[8] = '\0';
    std::printf("%s\n", buf);
}

static inline void print_str_large(OrionVM* vm, RawValue v) {
    uint8_t* rawPtr = vm->heapPtrFromAddr(v);
    auto* header = reinterpret_cast<const StringHeader*>(rawPtr);
    uint64_t length = header->length;
    const char* payload = reinterpret_cast<const char*>(rawPtr + sizeof(StringHeader));

    std::fwrite(payload, 1, static_cast<size_t>(length), stdout);
    std::fputc('\n', stdout);
}

static inline void print_i32(OrionVM* /*vm*/, RawValue v) {
    std::printf("%d\n", static_cast<int32_t>(v));
}

static inline void print_i64(OrionVM* /*vm*/, RawValue v) {
    std::printf("%lld\n", static_cast<long long>(v));
}

static inline void print_f32(OrionVM* /*vm*/, RawValue v) {
    float f;
    std::memcpy(&f, &v, sizeof(f));
    std::printf("%f\n", f);
}

static inline void print_f64(OrionVM* /*vm*/, RawValue v) {
    double d;
    std::memcpy(&d, &v, sizeof(d));
    std::printf("%lf\n", d);
}

static inline void print_bool(OrionVM* /*vm*/, RawValue v) {
    std::printf("%s\n", v ? "true" : "false");
}

static inline void print_i8(OrionVM* /*vm*/, RawValue v) {
    std::printf("%d\n", static_cast<int8_t>(v));
}

static inline void print_i16(OrionVM* /*vm*/, RawValue v) {
    std::printf("%d\n", static_cast<int16_t>(v));
}

static inline void print_char(OrionVM* /*vm*/, RawValue v) {
    const auto c = static_cast<uint32_t>(v);
    char utf8[5] = {0};
    if (c <= 0x7F) {
        utf8[0] = static_cast<char>(c);
    } else if (c <= 0x7FF) {
        utf8[0] = static_cast<char>(0xC0 | ((c >> 6) & 0x1F));
        utf8[1] = static_cast<char>(0x80 | (c & 0x3F));
    } else if (c <= 0xFFFF) {
        utf8[0] = static_cast<char>(0xE0 | ((c >> 12) & 0x0F));
        utf8[1] = static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        utf8[2] = static_cast<char>(0x80 | (c & 0x3F));
    } else {
        utf8[0] = static_cast<char>(0xF0 | ((c >> 18) & 0x07));
        utf8[1] = static_cast<char>(0x80 | ((c >> 12) & 0x3F));
        utf8[2] = static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        utf8[3] = static_cast<char>(0x80 | (c & 0x3F));
    }
    std::printf("%s\n", utf8);
}

static constexpr PrintFn printTable[256] = {
    nullptr, print_i32, print_i64, print_f32, print_f64, print_bool,
    print_str_small, print_i8, print_i16, print_char, print_str_large
};

#endif //PRINT_HANDLERS_H
