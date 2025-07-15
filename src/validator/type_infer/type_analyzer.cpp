//
// Created by home on 23/04/25.
//

#include "type_analyzer.h"

#include <iostream>
#include <set>

#include "sizes.h"


std::string TypeInfer::currentExtendedArrayType = "void";
std::string TypeInfer::currentArrayType = "void";
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

void TypeInfer::visitCharLiteralExpr(CharLiteralExpr *node) {
    currentType = "char";
}

void TypeInfer::visitStringLiteralExpr(StringLiteralExpr *node) {
    currentType = "str";
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


    if (arithmeticOps.contains(op) ||
        bitwiseOps.contains(op) || comparisonOps.contains(op) || logicalOps.contains(op)) {
        if (arithmeticOps.contains(op) && (left == "bool" || right == "bool")) {
            throw std::runtime_error("Arithmetic operators cannot be used with boolean types: " + left + " and " + right);
        }
        if (typePrecedence.contains(left) && typePrecedence.contains(right)) {
            const std::string resultType = promoteNumericTypes(left, right);
            node->type = resultType;
            if (comparisonOps.contains(op) || logicalOps.contains(op)) currentType = "bool";
            else currentType = resultType;
            return;
        }
        throw std::runtime_error("Arithmetic operators require definided types, got: " + left + " and " + right);
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


std::string unwrapArrayType(const std::string& t) {
    const std::string prefix = "array<";
    if (!t.starts_with(prefix)) return t;

    size_t first = t.find('<');
    size_t last = t.rfind('>');
    if (first == std::string::npos || last == std::string::npos || last <= first)
        throw std::runtime_error("Invalid array type format: " + t);

    std::string inner = t.substr(first + 1, last - first - 1);

    size_t pos = inner.find('<');
    if (pos != std::string::npos) {
        return inner.substr(0, pos);
    }

    return inner;
}

void TypeInfer::visitIndexAccessExpr(IndexAccessExpr *node) {
    node->index->accept(*this);
    node->base->accept(*this);

    std::string baseType;

    if (auto* baseArr = dynamic_cast<ArrayLiteralNode*>(node->base.get())) {
        baseType = baseArr->extendType;
    } else if (auto* baseIdx = dynamic_cast<IndexAccessExpr*>(node->base.get())) {
        baseType = baseIdx->baseType;
        if (baseType.starts_with("array<")) {
            baseType = baseType.substr(baseType.find('<') + 1, baseType.length() - baseType.find('<') - 2);
        }
    } else if (auto* ident = dynamic_cast<IdentifierExprNode*>(node->base.get())) {
        auto varInfo = lookupVariable(ident->name);

        baseType = varInfo.arrayType.empty() || varInfo.arrayType == "void" ? varInfo.type : varInfo.arrayType;
    }
    else {
        throw std::runtime_error("Base of index access must be an array or identifier");
    }

    std::string innerType = unwrapArrayType(baseType);


    node->baseType = baseType;
    node->type = innerType;


    if (baseType.starts_with("array<")) {
        currentType = innerType;
        node->type = "array";
    } else {
        currentType = innerType;
        node->type = baseType;
    }
}


void TypeInfer::visitArrayLiteralNode(ArrayLiteralNode *node) {
    if (node->elements.empty())
        throw std::runtime_error("Array literal cannot be empty");

    node->elements[0]->accept(*this);
    std::string baseType;
    if (auto* arr = dynamic_cast<ArrayLiteralNode*>(node->elements[0].get())) {
        baseType = arr->extendType;
    } else {
        baseType = currentType;
    }

    for (size_t i = 1; i < node->elements.size(); ++i) {
        node->elements[i]->accept(*this);

        std::string elemType;
        if (auto* arr = dynamic_cast<ArrayLiteralNode*>(node->elements[i].get())) {
            elemType = arr->extendType;
        } else {
            elemType = currentType;
        }

        if (elemType != baseType) {
            throw std::runtime_error(
                "Inconsistent element types in array literal: " + baseType + " vs " + elemType
            );
        }
    }

    node->elemType = baseType.starts_with("array<") ? "array" : baseType;
    node->extendType = "array<" + baseType + ">";
    node->type = "array";
    currentType = "array";

}



void TypeInfer::visitCastingExpressionNode(CastingExpressionNode *node) {
    node->expression->accept(*this);
    node->operandType = currentType;
    currentType = node->targetType;
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


