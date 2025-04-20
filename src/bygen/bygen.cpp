//
// Created by home on 19/04/25.
//

#include "bygen.h"
#include "bygen_expression.h"

std::vector<std::string> ByGen::generate(const std::vector<std::unique_ptr<ASTNode>>& root) {
    std::vector<std::string> bytecode;

    for (const auto& node : root) {

        const auto type = node.get();

        if (const auto exprState =  dynamic_cast<ExpressionStatementNode*>(type)) {
            auto exprBytecode = BygenExpression::generateExpression(exprState->expression);
            bytecode.insert(bytecode.end(), exprBytecode.begin(), exprBytecode.end());
            continue;
        }

        if (const auto expr =  dynamic_cast<ExpressionNode*>(type)) {
            auto exprBytecode =  BygenExpression::generateExpression(std::unique_ptr<ExpressionNode>(expr));
            bytecode.insert(bytecode.end(), exprBytecode.begin(), exprBytecode.end());
            continue;
        }

    }

    return bytecode;
}
