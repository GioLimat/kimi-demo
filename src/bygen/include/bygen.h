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
    std::unordered_map<std::string, uint32_t> globalVariables;
    size_t globalNextId = 0;
    size_t nextId = 0;
    std::vector<uint8_t> bytecode;
    std::vector<std::string> ir;
    [[nodiscard]] std::vector<std::string> splitBySpace(const std::string& str) const;
    uint32_t getIdentifierId(const std::string& name);
    void declareIdentifier(const std::string& name);
    void pushULEB128Identifier(uint32_t val);
public:
    ByGen(std::vector<std::string> ir);
    std::vector<uint8_t>  generate();
};

#endif //BYGEN_H
