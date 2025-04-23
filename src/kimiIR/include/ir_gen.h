//
// Created by home on 19/04/25.
//

#ifndef IR_GEN_H
#define IR_GEN_H

#include <vector>
#include "ast.h"
#include <string>

class IRGen {
public:
    static std::vector<std::string> generate(const std::unique_ptr<AST> &root);
};

#endif //IR_GEN_H
