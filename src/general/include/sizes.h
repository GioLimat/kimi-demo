//
// Created by home on 17/05/25.
//

#ifndef SIZES_H
#define SIZES_H
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, int> typePrecedence = {
    {"char", 2},

    {"u8", 0},
    {"u16", 1},
    {"u32", 2},
    {"u64", 3},

    {"i8", 0},
    {"i16", 1},
    {"i32", 2},
    {"i64", 3},
    {"f32", 4},
    {"f64", 5},

    {"str", 7},
    {"bool", 8},
    {"void", 9},
};



const std::vector<std::string> intSizes = {"64", "32", "16", "8"};
const std::vector<std::string> floatSizes = {"64", "32"};


#endif //SIZES_H
