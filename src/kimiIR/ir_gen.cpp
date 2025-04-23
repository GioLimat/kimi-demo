//
// Created by home on 19/04/25.
//

#include "ir_gen.h"

#include <iostream>
#include <ir_gen_declaration.h>
#include <ir_gen_expression.h>

std::vector<std::string> IRGen::generate(const std::unique_ptr<AST> &root) {
    std::vector<std::string> bytecode;

    for (const auto& node : root->children) {

        const auto type = node.get();

        if (const auto exprState =  dynamic_cast<ExpressionStatementNode*>(type)) {
            auto exprBytecode = IRGenExpression::generateExpression(exprState->expression.get());
            bytecode.insert(bytecode.end(), exprBytecode.begin(), exprBytecode.end());
            continue;
        }

        if (const auto expr =  dynamic_cast<ExpressionNode*>(type)) {

            auto exprBytecode =  IRGenExpression::generateExpression(&*expr);
            bytecode.insert(bytecode.end(), exprBytecode.begin(), exprBytecode.end());
            continue;
        }

        if (const auto decl = dynamic_cast<StatementNode*>(type)) {
            auto declBytecode = IRGenDeclaration::generateDeclaration(decl);
            bytecode.insert(bytecode.end(), declBytecode.begin(), declBytecode.end());
            continue;
        }

    }
    return bytecode;
}
