//
// Created by home on 19/04/25.
//

#ifndef IR_GEN_DECLARATION_H
#define IR_GEN_DECLARATION_H

#include "ast.h"
#include <vector>
#include <string>

class IRGenDeclaration {
public:
    static std::vector<std::string> varDeclaration(const VarDeclarationNode *varDeclaration);
    static std::vector<std::string> generateDeclaration(const StatementNode* statement);
};

#endif //IR_GEN_DECLARATION_H
