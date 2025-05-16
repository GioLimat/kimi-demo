//
// Created by home on 19/04/25.
//

#ifndef IR_GEN_H
#define IR_GEN_H

#include <stack>
#include <vector>
#include "ast.h"
#include <string>
#include <unordered_map>

#include "semantic_analyzer.h"
#include "visitor.h"

class IRGen : public DefaultASTVisitor {
    const std::unique_ptr<AST> &root;

    void visitAssignmentExpr(AssignmentExprNode *node) override;
    void visitBinaryExpr(BinaryExprNode* expression) override;
    void visitNumber(NumberNode *number) override;
    void visitIdentifier(IdentifierExprNode *identifier) override;
    void visitVarDeclaration(VarDeclarationNode *varDeclaration) override;
    void visitFunctionDeclaration(FunctionDeclarationNode *functionDeclaration) override;
    void visitBlockStatement(BlockStatementNode *node) override;
    void visitIfStatement(IfStatementNode *node) override;
    void visitWhileStatement(WhileStatementNode *node) override;
    void visitDoWhileStatement(DoWhileStatementNode *node) override;
    void visitPrintln(PrintlnStatementNode *node) override;
    void visitCallFunction(CallFunctionNode *node) override;
    void visitReturnStatement(ReturnStatementNode *node) override;
    void visitUnaryExpr(UnaryExprNode *node) override;
    void visitGenericExpressionNode(GenericExpressionNode *node) override;
    void visitPostFixExpr(PostFixExprNode *node) override;
    void visitBoolean(BooleanNode *node) override;

    protected:
    std::vector<std::string> bytecode;
    SemanticAnalyzer::Scope scopes;
    size_t currentLabel = 1;


    template <typename  T>
    T lookup(const std::string& name, const std::string& message) const {
        auto tempScopes = scopes;

        while (!tempScopes.empty()) {
            if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
                return std::get<T>(currentScope.at(name));
            }
            tempScopes.pop();
        }

        throw std::runtime_error(message);
    }
public:
    explicit IRGen(const std::unique_ptr<AST> &root);
    std::vector<std::string> generate();
};

#endif //IR_GEN_H
