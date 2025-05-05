//
// Created by home on 19/04/25.
//

#include "ir_gen.h"

#include <iostream>

IRGen::IRGen(const std::unique_ptr<AST> &root) : root(root) {
}


std::vector<std::string> IRGen::generate() {
    scopes.emplace();
    for (const auto& node : root->children) {
        node->accept(*this);
    }
    return bytecode;
}
