//
// Created by home on 20/04/25.
//

#ifndef INSTRUCTIONS_LLOC_H
#define INSTRUCTIONS_LLOC_H


#include "executor.h"
#include "instructions.h"

class StoreI32Instruction : public Instruction {
private:
    std::string operand;
public:
    explicit StoreI32Instruction(std::string  operand);
    void execute(ShinExecutor& executor) override;
};

class LoadI32Instruction : public Instruction {
private:
    std::string operand;
public:
    explicit LoadI32Instruction(std::string  operand);
    void execute(ShinExecutor& executor) override;
};

class ConstI32Instruction : public Instruction {
private:
    int value;
public:
    explicit ConstI32Instruction(int value);
    void execute(ShinExecutor& executor) override;
};




#endif //INSTRUCTIONS_LLOC_H
