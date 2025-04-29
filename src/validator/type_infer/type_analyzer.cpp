//
// Created by home on 23/04/25.
//

#include "type_analyzer.h"

#include <iostream>
#include <set>

std::stack<std::unordered_map<std::string, SemanticAnalyzer::VariableInfo>>* TypeInfer::scopes = nullptr;

std::string TypeInfer::analyzeExpression(ExpressionNode* expr,
    std::stack<std::unordered_map<std::string, SemanticAnalyzer::VariableInfo>>* declared) {

    scopes = declared;
    TypeInfer infer;
    expr->accept(infer);
    return infer.currentType;
}

void TypeInfer::visitNumber(NumberNode* node) {
    currentType = node->type;
}

void TypeInfer::visitBoolean(BooleanNode* node) {
    currentType = "bool";
}

void TypeInfer::visitIdentifier(IdentifierExprNode* node) {
    try {
        std::string type = lookupVariable(node->name).type;
        node->type = type;
        currentType = type;
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Usage of variable " + node->name + " was not declared");
    }
}

void TypeInfer::visitBinaryExpr(BinaryExprNode* node) {
    const std::set<std::string> arithmeticOps = {"+", "-", "*", "/", "%"};
    const std::set<std::string> logicalOps = {"&&", "||"};
    const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};

    node->left->accept(*this);
    std::string left = currentType;

    node->right->accept(*this);
    std::string right = currentType;

    const std::string& op = node->op;

    if (arithmeticOps.contains(op)) {
        if ((left == "i32" || left == "f64") && (right == "i32" || right == "f64")) {
            node->type = (left == "f64" || right == "f64") ? "f64" : "i32";
            currentType = node->type;
            return;
        }
        throw std::runtime_error("Arithmetic operators require numeric types, got: " + left + " and " + right);
    }

    if (logicalOps.contains(op)) {
        if (left == "bool" && right == "bool") {
            node->type = "bool";
            currentType = "bool";
            return;
        }
        throw std::runtime_error("Logical operators require boolean operands, got: " + left + " and " + right);
    }

    if (comparisonOps.contains(op)) {
        if (left == right) {
            node->type = "bool";
            currentType = "bool";
            return;
        }
        throw std::runtime_error("Cannot compare different types: " + left + " and " + right);
    }

    throw std::runtime_error("Unknown binary operator: " + op);
}

SemanticAnalyzer::VariableInfo TypeInfer::lookupVariable(const std::string &name) {
    if (!scopes) {
        throw std::runtime_error("Scope stack is not initialized in TypeInfer");
    }

    auto copy = *scopes;
    while (!copy.empty()) {
        const auto& scope = copy.top();
        if (scope.contains(name)) {
            return scope.at(name);
        }
        copy.pop();
    }

    throw std::runtime_error("Variable " + name + " not declared in any scope");
}