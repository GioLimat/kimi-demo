//
// Created by home on 02/05/25.
//

#ifndef BYGEN_H
#define BYGEN_H
#include <string>
#include <vector>

class ByGen {
    std::vector<std::string> ir;
    size_t current;
    std::string peek();
    std::string advance();
    [[nodiscard]] size_t findEndBlock() const;
    std::vector<std::string> splitBySpace(const std::string& str) const;
public:
    ByGen(std::vector<std::string> ir);
    std::vector<uint8_t>  generate();
};

#endif //BYGEN_H
