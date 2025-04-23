//
// Created by home on 23/04/25.
//
#include "semantic_analyzer.h"


void SemanticAnalyzer::analyze(const AST& ast) {
    enterScope();
    for (const auto& node : ast.children) {
        if (auto var = dynamic_cast<VarDeclarationNode*>(node.get())) {
            visitVarDeclaration(var);
            declareVariable(var->name, var->declaredType, var->isConst);
        }
    }

}


void SemanticAnalyzer::declareVariable(const std::string& name, const std::string& type, bool isConst) {
    if (scopes.top().contains(name)) {
        throw std::runtime_error("Variable " + name + " already declared in this scope");
    }
    scopes.top()[name] = type;
}


void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    scopes.pop();
}

