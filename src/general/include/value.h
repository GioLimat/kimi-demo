//
// Created by home on 03/05/25.
//

#ifndef VALUE_H
#define VALUE_H
#include <cstdint>
#include <variant>


struct Char {
    uint32_t code;
};


using ValueT = std::variant<int32_t, double, int8_t, bool, int16_t, int64_t, float, char32_t, uint8_t, uint16_t, uint32_t, uint64_t>;

#endif //VALUE_H
