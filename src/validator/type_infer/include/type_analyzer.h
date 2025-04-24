//
// Created by home on 23/04/25.
//

#ifndef TYPE_ANALYZER_H
#define TYPE_ANALYZER_H

#include "ast.h"
#include "semantic_analyzer.h"


class TypeInfer {
private:
    static std::stack<std::unordered_map<std::string, SemanticAnalyzer::VariableInfo>>* scopes;
    static SemanticAnalyzer::VariableInfo lookupVariable(const std::string &name);
public:
    static std::string analyzeExpression(const ExpressionNode* expr, std::stack<std::unordered_map<std::string, SemanticAnalyzer::VariableInfo>> *declared);
};

#endif //TYPE_ANALYZER_H
