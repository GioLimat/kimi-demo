//
// Created by home on 19/04/25.
//

#ifndef BYGEN_DECLARATION_H
#define BYGEN_DECLARATION_H

#include "bygen.h"

class ByGenDeclaration {
public:
    static std::vector<std::string> generateDeclaration(const std::unique_ptr<StatementNode>& statement);
private:
    static std::vector<std::string> varDeclaration(const VarDeclarationNode* varDeclaration);
    static std::vector<std::string> functionDeclaration(FunctionDeclarationNode* functionDeclaration);
};

#endif //BYGEN_DECLARATION_H
