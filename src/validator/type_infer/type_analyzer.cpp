//
// Created by home on 23/04/25.
//

#include "type_analyzer.h"

#include <iostream>
#include <set>

#include "sizes.h"



SemanticAnalyzer::Scope* TypeInfer::scopes = nullptr;

std::string promoteNumericTypes(const std::string& left, const std::string& right) {
    auto l = typePrecedence.find(left);
    auto r = typePrecedence.find(right);

    if (l == typePrecedence.end() || r == typePrecedence.end()) {
        throw std::runtime_error("Unknown numeric types for promotion: " + left + ", " + right);
    }

    return (l->second > r->second) ? left : right;
}

std::string TypeInfer::analyzeExpression(ExpressionNode* expr,
    SemanticAnalyzer::Scope* declared) {

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

void TypeInfer::visitCallFunction(CallFunctionNode *node) {
    currentType = node->returnType;
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
    const std::set<std::string> bitwiseOps = {"&", "|", "^", "<<", ">>"};
    const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};

    node->left->accept(*this);
    const std::string left = currentType;

    node->right->accept(*this);
    const std::string right = currentType;

    const std::string& op = node->op;

    if (arithmeticOps.contains(op) || bitwiseOps.contains(op)) {
        if (typePrecedence.contains(left) && typePrecedence.contains(right)) {
            const std::string resultType = promoteNumericTypes(left, right);
            node->type = resultType;
            currentType = resultType;
            return;
        }
        throw std::runtime_error("Arithmetic operators require numeric types, got: " + left + " and " + right);
    }

    if (logicalOps.contains(op) || comparisonOps.contains(op)) {
        currentType = "bool";
        node->type = "bool";
        return;
    }

    throw std::runtime_error("Unknown binary operator: " + op);
}


void TypeInfer::visitUnaryExpr(UnaryExprNode *node) {
    node->operand->accept(*this);
    node->type = currentType;
}


void TypeInfer::visitPostFixExpr(PostFixExprNode *node) {
    node->operand->accept(*this);
    node->type = currentType;
}


SemanticAnalyzer::VariableInfo TypeInfer::lookupVariable(const std::string &name) {
    if (!scopes) {
        throw std::runtime_error("Scope stack is not initialized in TypeInfer");
    }

    auto copy = *scopes;
    while (!copy.empty()) {
        const auto& scope = copy.top();
        if (scope.contains(name)) {
            return std::get<SemanticAnalyzer::VariableInfo>(scope.at(name));
        }
        copy.pop();
    }

    throw std::runtime_error("Variable " + name + " not declared in any scope");
}