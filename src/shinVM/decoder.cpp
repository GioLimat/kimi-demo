//
// Created by home on 20/04/25.
//


#include "decoder.h"

#include <iostream>

#include "instructions_arithmetic.h"
#include "instructions_lloc.h"
#include <stdexcept>

#include "instructions_macros.h"

std::unique_ptr<Instruction> Decoder::decode(const std::string& instruction) {
    const auto opcode = instruction.substr(0, instruction.find(' '));
    const auto operand = instruction.find(' ') != std::string::npos
                         ? instruction.substr(instruction.find(' ') + 1)
                         : "";

    if (opcodeToString.at(Opcode::ADD_I32) == opcode) {
        return std::make_unique<AddI32Instruction>();
    }
    if (opcodeToString.at(Opcode::SUB_I32) == opcode) {
        return std::make_unique<SubI32Instruction>();
    }
    if (opcodeToString.at(Opcode::MUL_I32) == opcode) {
        return std::make_unique<MulI32Instruction>();
    }
    if (opcodeToString.at(Opcode::DIV_I32) == opcode) {
        return std::make_unique<DivI32Instruction>();
    }
    if (opcodeToString.at(Opcode::LOAD) == opcode) {
        return std::make_unique<LoadI32Instruction>(operand);
    }
    if (opcodeToString.at(Opcode::STORE) == opcode) {
        return std::make_unique<StoreI32Instruction>(operand);
    }
    if (opcodeToString.at(Opcode::CONST_I32) == opcode) {
        try {
            int value = std::stoi(operand);
            return std::make_unique<ConstI32Instruction>(value);
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Invalid operand for CONST_I32");
        }
    }
    if (opcodeToString.at(Opcode::PRINTLN_I32) == opcode) {
        return std::make_unique<PrintLnI32Instruction>();
    }
    return nullptr;
}
