//
// Created by home on 23/04/25.
//

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include <stack>
#include <visitor.h>
#include <unordered_map>

class SemanticAnalyzer : public DefaultASTVisitor {
public:
    void analyze(const AST& ast);
    struct VariableInfo {
        std::string type;
        bool isConst;
    };
private:

    std::stack<std::unordered_map<std::string, VariableInfo>> scopes;

    void enterScope();
    void exitScope();
    void declareVariable(const std::string& name, const std::string& type, bool isConst);
    [[nodiscard]] VariableInfo lookupVariable(const std::string& name) const;


    void visitVarDeclaration(VarDeclarationNode* var) override;
    void visitAssignmentExpr(AssignmentExprNode* expr) override;
    void visitCallFunction(CallFunctionNode* call) override;
    void visitBinaryExpr(BinaryExprNode *node) override;

};

#endif //SEMANTIC_ANALYZER_H
