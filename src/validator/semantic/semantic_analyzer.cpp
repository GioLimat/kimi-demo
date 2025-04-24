//
// Created by home on 23/04/25.
//
#include "semantic_analyzer.h"

#include <iostream>

#include "type_analyzer.h"


void SemanticAnalyzer::analyze(const AST& ast) {
    enterScope();
    for (const auto& node : ast.children) {
        if (const auto var = dynamic_cast<VarDeclarationNode*>(node.get())) {
            visitVarDeclaration(var);
        }
        else if (const auto assi = dynamic_cast<AssignmentExprNode*>(node.get())) {
            visitAssignmentExpr(assi);
        }
        else if (const auto call = dynamic_cast<CallFunctionNode*>(node.get())) {
            visitCallFunction(call);
        }
        else if (const auto expr = dynamic_cast<ExpressionNode*>(node.get())) {
            TypeInfer::analyzeExpression(expr, &scopes);
        }
        else if (const auto exprStatement = dynamic_cast<ExpressionStatementNode*>(node.get())) {
            TypeInfer::analyzeExpression(exprStatement->expression.get(), &scopes);
        }

    }

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

void SemanticAnalyzer::visitAssignmentExpr(const AssignmentExprNode* expr) {
    TypeInfer::analyzeExpression(expr, &scopes);

    const auto varInfo = lookupVariable(expr->name);
    if (varInfo.isConst) {
        throw std::runtime_error("Cannot assign to const variable " + expr->name);
    }

    const std::string valueType = TypeInfer::analyzeExpression(expr->value.get(), &scopes);
    if (valueType != varInfo.type) {
        throw std::runtime_error("Type mismatch in assignment to '" + expr->name +
                                 "': expected " + varInfo.type + ", got " + valueType);
    }
}

void SemanticAnalyzer::visitCallFunction(CallFunctionNode *call) {

}


void SemanticAnalyzer::declareVariable(const std::string& name, const std::string& type, const bool isConst) {
    if (scopes.top().contains(name)) {
        throw std::runtime_error("Variable " + name + " already declared in this scope");
    }
    scopes.top()[name] = VariableInfo{type, isConst};
}


SemanticAnalyzer::VariableInfo SemanticAnalyzer::lookupVariable(const std::string &name) {
    auto tempScopes = scopes;

    while (!tempScopes.empty()) {
        if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
            return currentScope.at(name);
        }
        tempScopes.pop();
    }

    throw std::runtime_error("Variable " + name + " not declared in any accessible scope");
}



void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    scopes.pop();
}

