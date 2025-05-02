//
// Created by home on 23/04/25.
//

#ifndef TYPE_ANALYZER_H
#define TYPE_ANALYZER_H

#include "ast.h"
#include "semantic_analyzer.h"


class TypeInfer : public DefaultASTVisitor {
private:
    static SemanticAnalyzer::Scope* scopes;
    static SemanticAnalyzer::VariableInfo lookupVariable(const std::string &name);
    mutable std::string currentType;

public:
    static std::string analyzeExpression(ExpressionNode* expr,
        SemanticAnalyzer::Scope* declared);

    void visitNumber(NumberNode* node) override;
    void visitIdentifier(IdentifierExprNode* node) override;
    void visitBoolean(BooleanNode* node) override;
    void visitBinaryExpr(BinaryExprNode* node) override;
};


#endif //TYPE_ANALYZER_H
