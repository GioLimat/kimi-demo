//
// Created by home on 03/05/25.
//

#ifndef VALUE_H
#define VALUE_H
#include <cstdint>
#include <variant>


using ValueT = std::variant<int32_t, double, int8_t, bool, int16_t, int64_t>;

#endif //VALUE_H
