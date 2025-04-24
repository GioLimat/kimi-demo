//
// Created by home on 10/04/25.
//

#ifndef AST_H
#define AST_H


#include <string>
#include <memory>
#include <vector>




class ASTNode {
public:
    virtual ~ASTNode() = default;
};


class AST {
public:
    std::vector<std::unique_ptr<ASTNode>> children;
    explicit AST(std::vector<std::unique_ptr<ASTNode>> children)
        : children(std::move(children)) {}
};


class ExpressionNode : public ASTNode {
    public:
    virtual ~ExpressionNode() = default;
};


class NumberNode : public ExpressionNode {
    public:
        std::string value;
        std::string type;
        explicit NumberNode(std::string value);
        explicit NumberNode(std::string value, std::string type);
};

class BooleanNode : public ExpressionNode {
public:
    bool value;
    explicit BooleanNode(bool value);
};

class IdentifierExprNode : public ExpressionNode {
public:
    std::string name;
    std::string type;
    explicit IdentifierExprNode(std::string name);
};

class BinaryExprNode : public ExpressionNode {
public:
    std::string op;
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
    std::string type;
    BinaryExprNode(std::string op, std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
};

class AssignmentExprNode : public ExpressionNode {
public:
    std::string name;
    std::unique_ptr<ExpressionNode> value;
    std::string type;

    explicit AssignmentExprNode(std::string name, std::unique_ptr<ExpressionNode> value);
};

class StatementNode : public ASTNode {};


class BlockStatementNode : public StatementNode {
public:
    std::vector<std::unique_ptr<StatementNode>> statements;
    explicit BlockStatementNode(std::vector<std::unique_ptr<StatementNode>> statements);
};


class ExpressionStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> expression;

    explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expression);
};

class VarDeclarationNode : public StatementNode {
public:
    bool isConst;
    std::string name;
    std::unique_ptr<ExpressionNode> initializer;
    std::string declaredType;
    std::string inferType;
    std::string type;
    VarDeclarationNode(std::string name, std::unique_ptr<ExpressionNode> initializer, bool isConst);
};

class FunctionDeclarationNode : public StatementNode {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<StatementNode>> body;
    FunctionDeclarationNode(std::string name, std::vector<std::string> parameters, std::vector<std::unique_ptr<StatementNode>> body);
};

class CallFunctionNode : public ExpressionNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
    std::string returnType;
    CallFunctionNode(std::string name, std::vector<std::unique_ptr<ExpressionNode>> arguments);
};

class UnaryExprNode : public ExpressionNode {
public:
    std::string op;
    std::unique_ptr<ExpressionNode> operand;
    std::string type;

    UnaryExprNode(std::string op, std::unique_ptr<ExpressionNode> operand);
};


class ReturnStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> returnValue;
    std::string returnType;
    explicit ReturnStatementNode(std::unique_ptr<ExpressionNode> returnValue);
};

class IfStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> condition;
    std::vector<std::unique_ptr<StatementNode>> thenBranch;
    std::unique_ptr<StatementNode> elseBranch;

    explicit IfStatementNode(std::unique_ptr<ExpressionNode> condition,
                    std::vector<std::unique_ptr<StatementNode>> thenBranch,
                    std::unique_ptr<StatementNode> elseBranch);
};


class WhileStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> body;

    explicit WhileStatementNode(std::unique_ptr<ExpressionNode> condition,
                       std::unique_ptr<BlockStatementNode> body);
};

class DoWhileStatementNode : public StatementNode {
  public:
    std::unique_ptr<WhileStatementNode> whileStatement;

    explicit DoWhileStatementNode(std::unique_ptr<WhileStatementNode> whileStatement);
};

class PrintlnStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> expression;
    explicit PrintlnStatementNode(std::unique_ptr<ExpressionNode> expression);
};

#endif // AST_H
