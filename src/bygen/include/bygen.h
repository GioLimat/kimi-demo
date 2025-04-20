//
// Created by home on 19/04/25.
//

#ifndef BYGEN_H
#define BYGEN_H

#include <vector>
#include "ast.h"
#include <string>

class ByGen {
public:
    static std::vector<std::string> generate(const std::vector<std::unique_ptr<ASTNode>>& root);
};

#endif //BYGEN_H
