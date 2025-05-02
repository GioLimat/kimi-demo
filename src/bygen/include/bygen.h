//
// Created by home on 02/05/25.
//

#ifndef BYGEN_H
#define BYGEN_H
#include <stack>
#include <string>
#include <vector>

class ByGen {
    std::stack<std::unordered_map<std::string, uint32_t>> symbolTable;
    uint32_t nextVarId = 0;
    std::vector<std::string> ir;
    [[nodiscard]] std::vector<std::string> splitBySpace(const std::string& str) const;
public:
    ByGen(std::vector<std::string> ir);
    std::vector<uint8_t>  generate();
};

#endif //BYGEN_H
