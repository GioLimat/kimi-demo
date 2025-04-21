//
// Created by home on 21/04/25.
//
#include "instructions_macros.h"

#include <iostream>

void PrintLnI32Instruction::execute(ShinExecutor& executor) {
    auto stack = executor.getStackManager();
    int value = stack.pop();
    std::cout << value << std::endl;
}
