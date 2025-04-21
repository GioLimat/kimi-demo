//
// Created by home on 20/04/25.
//

#ifndef SHIN_VM_H
#define SHIN_VM_H

#include "instructions.h"
#include <string>
#include <utility>
#include <vector>

#include "executor.h"
#include "instructions.h"

class ShinVm final : public IExecutionUnit{

public:
    void execute() override;
    void reset() override;
    explicit ShinVm(const std::vector<std::string> &bytecode);
private:
    std::vector<std::string> bytecode;
};

#endif //SHIN_VM_H
