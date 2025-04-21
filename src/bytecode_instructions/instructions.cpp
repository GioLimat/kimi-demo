//
// Created by home on 20/04/25.
//

#include "instructions.h"

#include <utility>

Instruction::Instruction(const Opcode opcode, std::string operand) : opcode(opcode), operand(std::move(operand)) {}

Instruction::Instruction(const Opcode opcode): opcode(opcode){}



Opcode Instruction::getOpcode() const {
    return opcode;
}

std::string Instruction::getOperand() const {
    return operand;
}

bool Instruction::hasOperand() const {
    return !operand.empty();
}

