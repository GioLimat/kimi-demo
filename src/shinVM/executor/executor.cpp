//
// Created by home on 20/04/25.
//

#include "executor.h"

#include <decoder.h>
#include <iostream>

void ShinExecutor::execute(const std::vector<std::string>& instructions) {
    for (const auto & instruction : instructions) {
        const auto code = Decoder::decode(instruction);
        code->execute(*this);
        std::cout << stackManager.top() << std::endl;
    }
}

StackManager& ShinExecutor::getStackManager() {
    return stackManager;
}

std::unordered_map<std::string, int>& ShinExecutor::getGlobalVariables() {
    return globalVariables;
}