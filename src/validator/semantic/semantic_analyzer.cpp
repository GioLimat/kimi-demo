//
// Created by home on 23/04/25.
//
#include "semantic_analyzer.h"

#include <iostream>

#include "type_analyzer.h"


void SemanticAnalyzer::analyze(const AST& ast) {
    enterScope();
    for (auto& node : ast.children) {
        node->accept(*this);
    }
}

void SemanticAnalyzer::visitBinaryExpr(BinaryExprNode *node) {
    TypeInfer::analyzeExpression(node, &scopes);
}

void SemanticAnalyzer::visitVarDeclaration(VarDeclarationNode* var) {
    std::string inferredType;

    if (var->initializer) {
        inferredType = TypeInfer::analyzeExpression(var->initializer.get(), &scopes);

        if (!var->declaredType.empty() && var->declaredType != inferredType) {
            throw std::runtime_error("Type mismatch in variable declaration: expected " + var->declaredType + ", got " + inferredType);
        }
    }
    else if (var->isConst) {
        throw std::runtime_error("Const variable " + var->name + " must be initialized");
    }
    else if (var->declaredType.empty()) {
        throw std::runtime_error("Variable " + var->name + " must have a type or be initialized, to type infer");
    }

    const std::string finalType = var->declaredType.empty() ? inferredType : var->declaredType;
    declareVariable(var->name, finalType, var->isConst);
    var->type = finalType;
}

void SemanticAnalyzer::visitAssignmentExpr(AssignmentExprNode* expr) {
    TypeInfer::analyzeExpression(expr, &scopes);

    const auto [type, isConst] = lookupVariable(expr->name);
    if (isConst) {
        throw std::runtime_error("Cannot assign to const variable " + expr->name);
    }

    if (const std::string valueType = TypeInfer::analyzeExpression(expr->value.get(), &scopes); valueType != type) {
        throw std::runtime_error("Type mismatch in assignment to '" + expr->name +
                                 "': expected " + type + ", got " + valueType);
    }
}

void SemanticAnalyzer::visitFunctionDeclaration(FunctionDeclarationNode *node) {
    declareFunction(node->name, node->parameters, node->parametersTypes);
    enterScope();
    for (auto& statement : node->body->statements) {
        statement->accept(*this);
    }
    exitScope();
}


void SemanticAnalyzer::visitExpressionStatement(ExpressionStatementNode *node) {
    TypeInfer::analyzeExpression(node->expression.get(), &scopes);
}




void SemanticAnalyzer::declareFunction(const std::string& name, const std::vector<std::string>& parameters,
                                        const std::vector<std::string>& parametersTypes) {
    if (scopes.top().contains(name)) {
        throw std::runtime_error("Function " + name + " already declared in this scope");
    }
    scopes.top()[name] = FunctionInfo{name, parameters, parametersTypes};
}

void SemanticAnalyzer::declareVariable(const std::string& name, const std::string& type, const bool isConst) {
    if (scopes.top().contains(name)) {
        throw std::runtime_error("Variable " + name + " already declared in this scope");
    }
    scopes.top()[name] = VariableInfo{type, isConst};
}


SemanticAnalyzer::VariableInfo SemanticAnalyzer::lookupVariable(const std::string &name) const {
    auto tempScopes = scopes;

    while (!tempScopes.empty()) {
        if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
            const auto& symbol = currentScope.at(name);
            if (!std::holds_alternative<VariableInfo>(symbol)) {
                throw std::runtime_error("Symbol '" + name + "' is not a variable.");
            }
            return std::get<VariableInfo>(symbol);
        }
        tempScopes.pop();
    }

    throw std::runtime_error("Variable " + name + " not declared in any accessible scope");
}

SemanticAnalyzer::FunctionInfo SemanticAnalyzer::lookupFunction(const std::string &name) const {
    auto tempScopes = scopes;

    while (!tempScopes.empty()) {
        if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
            return std::get<FunctionInfo>(currentScope.at(name));
        }
        tempScopes.pop();
    }

    throw std::runtime_error("Function " + name + " not declared in any accessible scope");
}



void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    scopes.pop();
}

