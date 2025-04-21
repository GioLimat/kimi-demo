//
// Created by home on 21/04/25.
//

#ifndef MACROS_H
#define MACROS_H

#include "instructions.h"

#include "executor.h"

class PrintLnI32Instruction : public Instruction {
public:
    void execute(ShinExecutor& executor) override;
};

#endif //MACROS_H
