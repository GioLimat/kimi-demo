//
// Created by home on 23/04/25.
//

#include "type_analyzer.h"

std::string TypeInfer::analyzeExpression(ExpressionNode *expr) {
    if (const auto number = dynamic_cast<NumberNode*>(expr)) {
        return number->type;
    }

    if (const auto binary = dynamic_cast<BinaryExprNode*>(expr)) {
        auto left = analyzeExpression(binary->left.get());

        if (const auto right = analyzeExpression(binary->right.get()); left != right) {
            if ((left == "i32" && right == "f64") || (left == "f64" && right == "i32")) {
                binary->type = "f64";
                return "f64";
            }
            throw std::runtime_error("Type mismatch in binary expression.");
        }

        binary->type = left;
        return left;
    }

    return "unknown";
}
