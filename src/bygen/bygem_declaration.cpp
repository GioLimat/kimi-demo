//
// Created by home on 19/04/25.
//

#include "bygen_declaration.h"
#include "bygen_expression.h"


std::vector<std::string> ByGenDeclaration::varDeclaration(const VarDeclarationNode *varDeclaration) {
    std::vector<std::string> bytecode;

    if (varDeclaration->initializer) {
        std::vector<std::string> expr = BygenExpression::generateExpression(varDeclaration->initializer);
        bytecode.insert(bytecode.end(), expr.begin(), expr.end());
    }

    const std::string mutConst = varDeclaration->isConst ? "_CONST" : "";
    bytecode.push_back("STORE" + mutConst + " " + varDeclaration->name);

    return bytecode;
}


std::vector<std::string> ByGenDeclaration::generateDeclaration(const std::unique_ptr<StatementNode> & statement) {
    std::vector<std::string> bytecode;

    if (auto var = dynamic_cast<VarDeclarationNode*>(statement.get())) {
        const auto decl = varDeclaration(var);
        if (!decl.empty()) bytecode.insert(bytecode.end(), decl.begin(), decl.end());
    }

    return bytecode;
}
