//
// Created by home on 02/05/25.
//
#include "bygen_instructions.h"

uint8_t ByMapper::getInstruction(const std::string& ir) {
    if (const auto it = instructionMap.find(ir); it != instructionMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown IR instruction");
}



uint8_t ByMapper::getType(const std::string &type) {
    if (const auto it = typeMap.find(type); it != typeMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown type");
}