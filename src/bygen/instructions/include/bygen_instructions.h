//
// Created by home on 02/05/25.
//

#ifndef BYGEN_INSTRUCTIONS_H
#define BYGEN_INSTRUCTIONS_H

#include <cstdint>
#include <stdexcept>

#include "ir_instructions.h"

#include <unordered_map>

inline std::unordered_map<std::string, uint8_t> instructionMap  = {
    {"CONST", 0x01},
    {"LOAD", 0x02},
    {"STORE", 0x03},
    {"PRINT", 0x04},
    {"FN", 0x05},
    {"FN_PARAM", 0x06},
    {"END_BLOCK", 0x07},
    {"INIT_BLOCK", 0x08},
    {"IF", 0x09}, //not used
    {"DO", 0x0A}, //not used
    {"WHILE", 0x0B}, //not used
    {"ELSE", 0x0C}, //not used
    {"END_CONDITION", 0x0D}, //not used




    { "ADD", 0x0E },
    { "SUB", 0x0F },
    { "MUL", 0x10 },
    { "DIV", 0x11 },
    { "MOD", 0x12 },
    { "GREATER", 0x13 },
    { "LESS", 0x14 },
    { "EQUAL", 0x15 },

    {"CALL", 0x16},
    {"CALL_END", 0x17},
    {"RET", 0x18},
    {"END_FN", 0x19},
    {"IF_FALSE", 0x1A},
    {"JMP", 0x1B},
    {"NEG", 0x1C},
    {"INC", 0x1D},
    {"DEC", 0x1E},

    {"POST_INC", 0x1F},
    {"POST_DEC", 0x20},

    {"GREATER_EQUAL", 0x21},
    {"LESS_EQUAL", 0x22},
    {"NOT_EQUAL", 0x23},

    {"AND", 0x24},
    {"OR", 0x25},
    {"NOT", 0x26},


    {"BIT_AND", 0x27},
    {"BIT_OR", 0x28},
    {"BIT_XOR", 0x29},
    {"SHIFT_LEFT", 0x2A},
    {"SHIFT_RIGHT", 0x2B},
    {"BIT_NOT", 0x2C},

    {"CONST_STR", 0x2D}, //not used
    {"ALLOC", 0x2E},
    {"FREE", 0x2F},
    {"REALLOC", 0x30},
    {"INDEX_ACCESS", 0x31},

    {"ALLOC_ARR", 0x32},
    {"EL_ARRAY_ASSIGN", 0x33}
};


inline std::unordered_map<std::string, uint8_t> typeMap = {
    {"i32", 0x01},
    {"i64", 0x02},
    {"f32", 0x03},
    {"f64", 0x04},
    {"bool", 0x05},

    {"i8", 0x07},
    {"i16", 0x08},
    {"char", 0x09},
    {"str", 0x0A},
    {"array", 0x0B}
};


inline std::unordered_map<std::string, uint8_t> typesSize = {
    {"i32", 32},
    {"i64", 64},
    {"f32", 32},
    {"f64", 64},
    {"bool", 8},

    {"i8", 8},
    {"i16", 16},
    {"char", 32}
};



class ByMapper {

public:
    static uint8_t getInstruction(const std::string &ir);
    static uint8_t getType(const std::string &type);
    static uint8_t getSize(const std::string &type);
};

#endif //BYGEN_INSTRUCTIONS_H
