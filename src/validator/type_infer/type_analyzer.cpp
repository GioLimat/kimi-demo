//
// Created by home on 23/04/25.
//

#include "type_analyzer.h"

#include <iostream>
#include <set>


std::string TypeInfer::analyzeExpression(const ExpressionNode* expr,
    std::stack<std::unordered_map<std::string, SemanticAnalyzer::VariableInfo>> *declared) {

    if (const auto number = dynamic_cast<const NumberNode*>(expr)) {
        return number->type;
    }
    if (const auto ident = const_cast<IdentifierExprNode*>(dynamic_cast<const IdentifierExprNode*>(expr))) {
        try {
            std::string type = declared->top().at(ident->name).type;
            ident->type = type;
            return type;
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Usage of variable " + ident->name + ", was not declared");
        }
    }
    if (dynamic_cast<const BooleanNode*>(expr)) {
        return "bool";
    }



    if (auto binary = dynamic_cast<BinaryExprNode*>(const_cast<ExpressionNode*>(expr))){
        const auto& op = binary->op;
        const std::string left = analyzeExpression(binary->left.get(), declared);
        const std::string right = analyzeExpression(binary->right.get(), declared);

        const std::set<std::string> arithmeticOps = {"+", "-", "*", "/", "%"};
        const std::set<std::string> logicalOps = {"&&", "||"};
        const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};

        if (arithmeticOps.contains(op)) {
            if ((left == "i32" || left == "f64") && (right == "i32" || right == "f64")) {
                if (left == "f64" || right == "f64") {
                    binary->type = "f64";
                    return "f64";
                }
                binary->type = "i32";
                return "i32";
            }
            throw std::runtime_error("Arithmetic operators require numeric types, got: " + left + " and " + right);
        }

        if (logicalOps.contains(op)) {
            if (left == "bool" && right == "bool") {
                binary->type = "bool";
                return "bool";
            }
            throw std::runtime_error("Logical operators require boolean operands, got: " + left + " and " + right);
        }

        if (comparisonOps.contains(op)) {
            if (left == right) {
                binary->type = "bool";
                return "bool";
            }
            throw std::runtime_error("Cannot compare different types: " + left + " and " + right);
        }

        throw std::runtime_error("Unknown binary operator: " + op);
    }

    return "unknown";
}

