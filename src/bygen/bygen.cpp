//
// Created by home on 02/05/25.
//

#include "bygen_instructions.h"
#include "bygen.h"

#include <iostream>
#include <ranges>
#include <sstream>
#include <bits/ostream.tcc>

ByGen::ByGen(std::vector<std::string> ir) : ir(std::move(ir)) {
    current = 0;
}

std::string ByGen::peek() {
    if (current >= ir.size()) {
        return ir[ir.size() - 1];
    }
    return ir[current];
}

std::string ByGen::advance() {
    if (current >= ir.size()) {
        return ir[ir.size() - 1];
    }
    return ir[current++];
}

size_t ByGen::findEndBlock() const {
    size_t depth = 0;
    for (size_t i = current; i < ir.size(); ++i) {
        if (ir[i] == "INIT_BLOCK") {
            depth++;
        } else if (ir[i] == "END_BLOCK") {
            depth--;
            if (depth == 0) {
                return i;
            }
        }
    }
    return INT64_MAX;
}


std::vector<std::string> ByGen::splitBySpace(const std::string& str) const {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}


std::vector<uint8_t>  ByGen::generate() {
    std::vector<uint8_t> bytecode;

    for (const auto& instruction : ir) {
        const auto parts  = splitBySpace(instruction);
        const auto& instructionType = parts[0];

        bytecode.push_back(ByMapper::getInstruction(instructionType));

        if (instructionType == "CONST") {
            uint32_t val = std::stoul(parts[1]);
            while (val >= 0x80) {
                bytecode.push_back(static_cast<uint8_t>((val & 0x7F) | 0x80));
                val >>= 7;
            }
            bytecode.push_back(static_cast<uint8_t>(val));
        }
    }

    return bytecode;
}
