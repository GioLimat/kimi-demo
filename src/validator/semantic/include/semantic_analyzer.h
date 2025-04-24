//
// Created by home on 23/04/25.
//

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include <stack>
#include <unordered_map>

class SemanticAnalyzer {
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
    VariableInfo lookupVariable(const std::string& name);

    void visitExpression(ExpressionNode* expr);
    void visitStatement(StatementNode* stmt);

    void visitVarDeclaration(VarDeclarationNode* var);
    void visitAssignmentExpr(const AssignmentExprNode* expr);
    void visitCallFunction(CallFunctionNode* call);
    void visitBlock(BlockStatementNode* block);
    void visitIfStatement(IfStatementNode* stmt);
    void visitWhileStatement(WhileStatementNode* stmt);
    void visitReturnStatement(ReturnStatementNode* stmt);

};

#endif //SEMANTIC_ANALYZER_H
