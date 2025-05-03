//
// Created by home on 19/04/25.
//

#ifndef IR_GEN_H
#define IR_GEN_H

#include <vector>
#include "ast.h"
#include <string>

#include "visitor.h"

class IRGen : public DefaultASTVisitor {
    const std::unique_ptr<AST> &root;

    void visitBinaryExpr(BinaryExprNode* expression) override;
    void visitNumber(NumberNode *number) override;
    void visitIdentifier(IdentifierExprNode *identifier) override;
    void visitVarDeclaration(VarDeclarationNode *varDeclaration) override;
    void visitFunctionDeclaration(FunctionDeclarationNode *functionDeclaration) override;
    void visitIfStatement(IfStatementNode *node) override;
    void visitWhileStatement(WhileStatementNode *node) override;
    void visitDoWhileStatement(DoWhileStatementNode *node) override;
    void visitPrintln(PrintlnStatementNode *node) override;
    void visitCallFunction(CallFunctionNode *node) override;

    protected:
    std::vector<std::string> bytecode;
public:
    explicit IRGen(const std::unique_ptr<AST> &root);
    std::vector<std::string> generate();
};

#endif //IR_GEN_H
