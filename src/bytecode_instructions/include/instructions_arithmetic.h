//
// Created by home on 20/04/25.
//

#ifndef ARITHMETIC_INSTRUCTIONS_H
#define ARITHMETIC_INSTRUCTIONS_H

#include "instructions.h"
#include "executor.h"

class AddI32Instruction : public Instruction {
public:
    void execute(ShinExecutor& executor) override;
};

class SubI32Instruction : public Instruction {
public:
    void execute(ShinExecutor& executor) override;
};

class MulI32Instruction : public Instruction {
public:
    void execute(ShinExecutor& executor) override;
};

class DivI32Instruction : public Instruction {
public:
    void execute(ShinExecutor& executor) override;
};

#endif // ARITHMETIC_INSTRUCTIONS_H
