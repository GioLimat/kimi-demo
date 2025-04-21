//
// Created by home on 20/04/25.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <string>
#include <unordered_map>

#include "executor.h"

enum class Opcode {
    ADD_I32,
    SUB_I32,
    MUL_I32,
    DIV_I32,
    LOAD,
    STORE,
    STORE_CONST,
    CONST_I32,
    PRINTLN_I32,
    UNDEFINED,
};


const std::unordered_map<Opcode, std::string> opcodeToString = {
    {Opcode::ADD_I32, "ADD_I32"},
    {Opcode::SUB_I32, "SUB_I32"},
    {Opcode::MUL_I32, "MUL_I32"},
    {Opcode::DIV_I32, "DIV_I32"},
    {Opcode::CONST_I32, "CONST_I32"},
    {Opcode::STORE, "STORE"},
    {Opcode::STORE_CONST, "STORE_CONST"},
    {Opcode::LOAD, "LOAD"},
    {Opcode::PRINTLN_I32, "PRINT_I32"}
};

class IExecutionUnit {
public:
    virtual ~IExecutionUnit() = default;

    virtual void execute() = 0;
    virtual void reset() = 0;
};



class Instruction {
private:
    Opcode opcode;
    std::string operand;


public:
    virtual ~Instruction() = default;

    Instruction()  = default;

    explicit Instruction(Opcode opcode, std::string operand);
    explicit Instruction(Opcode opcode);
    [[nodiscard]] Opcode getOpcode() const;
    [[nodiscard]] std::string getOperand() const;
    [[nodiscard]] bool hasOperand() const;

    virtual void execute(ShinExecutor& executor) = 0;
};



#endif //INSTRUCTIONS_H
