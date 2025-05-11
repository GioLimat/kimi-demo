//
// Created by home on 02/05/25.
//

#ifndef BYGEN_INSTRUCTIONS_H
#define BYGEN_INSTRUCTIONS_H

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
    {"IF", 0x09},
    {"DO", 0x0A},
    {"WHILE", 0x0B},
    {"ELSE", 0x0C},
    {"END_CONDITION", 0x0D},




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

    {"GLOAD", 0x19},

    {"IF_FALSE", 0x20},
    {"JMP", 0x21},
};


inline std::unordered_map<std::string, uint8_t> typeMap = {
    {"i32", 0x01},
    {"i64", 0x02},
    {"f32", 0x03},
    {"f64", 0x04},
    {"bool", 0x05},
    {"string", 0x06}
};



class ByMapper {

public:
    static uint8_t getInstruction(const std::string &ir);
    static uint8_t getType(const std::string &type);
};

#endif //BYGEN_INSTRUCTIONS_H
