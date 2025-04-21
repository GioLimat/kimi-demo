//
// Created by home on 20/04/25.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <string>
#include <unordered_map>
#include <vector>

#include "stack_manager.h"

class ShinExecutor {
private:
    StackManager stackManager;
    std::unordered_map<std::string, int> globalVariables;
public:
    void execute(const std::vector<std::string>& instructions);
    StackManager& getStackManager();
    std::unordered_map<std::string, int>& getGlobalVariables();
};

#endif //EXECUTOR_H
