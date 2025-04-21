//
// Created by home on 20/04/25.
//
#include "instructions_arithmetic.h"
#include <stdexcept>

void AddI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int b = stack.pop();
    int a = stack.pop();
    stack.push(a + b);
}

void SubI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int b = stack.pop();
    int a = stack.pop();
    stack.push(a - b);
}

void MulI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int b = stack.pop();
    int a = stack.pop();
    stack.push(a * b);
}

void DivI32Instruction::execute(ShinExecutor& executor) {
    StackManager& stack = executor.getStackManager();
    int b = stack.pop();
    int a = stack.pop();
    if (b == 0) throw std::runtime_error("division by zero");
    stack.push(a / b);
}
