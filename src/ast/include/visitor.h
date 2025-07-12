//
// Created by home on 29/04/25.
//

#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"


class ASTVisitor {
public:
    virtual void visitVarDeclaration(VarDeclarationNode* node) = 0;
    virtual void visitAssignmentExpr(AssignmentExprNode* node) = 0;
    virtual void visitCallFunction(CallFunctionNode* node) = 0;
    virtual void visitExpressionNode(ExpressionNode* node) = 0;
    virtual void visitStatementNode(StatementNode* node) = 0;
    virtual void visitExpressionStatement(ExpressionStatementNode* node) = 0;
    virtual void visitBinaryExpr(BinaryExprNode* node) = 0;
    virtual void visitNumber(NumberNode* node) = 0;
    virtual void visitIdentifier(IdentifierExprNode* node) = 0;
    virtual void visitUnaryExpr(UnaryExprNode* node) = 0;
    virtual void visitBoolean(BooleanNode* node) = 0;
    virtual void visitBlockStatement(BlockStatementNode* node) = 0;
    virtual void visitFunctionDeclaration(FunctionDeclarationNode* node) = 0;
    virtual void visitReturnStatement(ReturnStatementNode* node) = 0;
    virtual void visitIfStatement(IfStatementNode* node) = 0;
    virtual void visitWhileStatement(WhileStatementNode* node) = 0;
    virtual void visitDoWhileStatement(DoWhileStatementNode* node) = 0;
    virtual void visitForStatement(ForStatementNode* node) = 0;
    virtual void visitPrintln(PrintlnStatementNode* node) = 0;
    virtual void visitGenericExpressionNode(GenericExpressionNode* node) = 0;
    virtual void visitPostFixExpr(PostFixExprNode* node) = 0;
    virtual void visitCharLiteralExpr(CharLiteralExpr* node) = 0;
    virtual void visitStringLiteralExpr(StringLiteralExpr* node) = 0;
    virtual void visitIndexAccessExpr(IndexAccessExpr* node) = 0;
    virtual void visitArrayLiteralNode(ArrayLiteralNode* node) = 0;
    virtual void visitAssignmentIndexExpr(AssignmentIndexExprNode* node) = 0;
    virtual void visitInsertCollection(InsertStatementNode* node) = 0;
    virtual void visitRemoveCollection(RemoveStatementNode* node) = 0;
    virtual ~ASTVisitor() = default;
};

class DefaultASTVisitor : public ASTVisitor {
public:
    void visitVarDeclaration(VarDeclarationNode*) override           {}
    void visitAssignmentExpr(AssignmentExprNode*) override           {}
    void visitCallFunction(CallFunctionNode*) override               {}
    void visitExpressionNode(ExpressionNode*) override               {}
    void visitStatementNode(StatementNode*) override                 {}
    void visitExpressionStatement(ExpressionStatementNode*) override {}
    void visitBinaryExpr(BinaryExprNode*) override                   {}
    void visitNumber(NumberNode*) override                           {}
    void visitIdentifier(IdentifierExprNode*) override               {}
    void visitUnaryExpr(UnaryExprNode*) override                     {}
    void visitBoolean(BooleanNode*) override                         {}
    void visitBlockStatement(BlockStatementNode*) override           {}
    void visitFunctionDeclaration(FunctionDeclarationNode*) override {}
    void visitReturnStatement(ReturnStatementNode*) override         {}
    void visitIfStatement(IfStatementNode*) override                 {}
    void visitWhileStatement(WhileStatementNode*) override           {}
    void visitDoWhileStatement(DoWhileStatementNode*) override       {}
    void visitForStatement(ForStatementNode*) override               {}
    void visitPrintln(PrintlnStatementNode*) override               {}
    void visitGenericExpressionNode(GenericExpressionNode* node) override {}
    void visitPostFixExpr(PostFixExprNode* node) override {}
    void visitCharLiteralExpr(CharLiteralExpr* node) override {}
    void visitStringLiteralExpr(StringLiteralExpr *node) override {};
    void visitIndexAccessExpr(IndexAccessExpr* node) override {};
    void visitArrayLiteralNode(ArrayLiteralNode *node) override {};
    void visitAssignmentIndexExpr(AssignmentIndexExprNode* node) override {};
    void visitInsertCollection(InsertStatementNode* node) override {};
    void visitRemoveCollection(RemoveStatementNode* node) override {};
};
#endif //VISITOR_H
