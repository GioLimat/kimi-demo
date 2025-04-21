//
// Created by home on 20/04/25.
//

#include "shin_VM.h"
#include "executor.h"
#include <sstream>

ShinVm::ShinVm(const std::vector<std::string> &bytecode) : bytecode(bytecode) {}

void ShinVm::execute() {
    ShinExecutor executor;
    executor.execute(bytecode);
}



void ShinVm::reset() {

}
