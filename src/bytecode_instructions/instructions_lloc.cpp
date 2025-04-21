//
// Created by home on 20/04/25.
//


#include "instructions_lloc.h"

#include <iostream>
#include <ostream>
#include <utility>

StoreI32Instruction::StoreI32Instruction(std::string  operand)
    : operand(std::move(operand)) {}

void StoreI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int value = stack.pop();
    executor.getGlobalVariables()[operand] = value;
}

LoadI32Instruction::LoadI32Instruction(std::string  operand)
    : operand(std::move(operand)) {}

void LoadI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int value = executor.getGlobalVariables()[operand];
    stack.push(value);
}

ConstI32Instruction::ConstI32Instruction(const int value)
    : value(value) {}

void ConstI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    stack.push(value);
}
