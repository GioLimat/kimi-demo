//
// Created by home on 10/04/25.
//

#ifndef AST_H
#define AST_H


#include <string>
#include <memory>
#include <vector>


class ASTVisitor;


class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void accept(ASTVisitor &visitor) = 0;
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
    void accept(ASTVisitor &visitor) override;

};


class NumberNode : public ExpressionNode {
    public:
        std::string value;
        std::string type;
        explicit NumberNode(std::string value);
        explicit NumberNode(std::string value, std::string type);
        void accept(ASTVisitor &visitor) override;
};

class BooleanNode : public ExpressionNode {
public:
    bool value;
    explicit BooleanNode(bool value);
    void accept(ASTVisitor &visitor) override;
};

class IdentifierExprNode : public ExpressionNode {
public:
    std::string name;
    std::string type;
    explicit IdentifierExprNode(std::string name);
    void accept(ASTVisitor &visitor) override;
};

class BinaryExprNode : public ExpressionNode {
public:
    std::string op;
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
    std::string type;
    explicit BinaryExprNode(std::string op, std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
    void accept(ASTVisitor &visitor) override;
};

class AssignmentExprNode : public ExpressionNode {
public:
    std::string name;
    std::unique_ptr<ExpressionNode> value;
    std::string type;

    explicit AssignmentExprNode(std::string name, std::unique_ptr<ExpressionNode> value);
    void accept(ASTVisitor &visitor) override;
};

class StatementNode : public ASTNode {
public:
    virtual ~StatementNode() = default;
    void accept(ASTVisitor &visitor) override;
};


class BlockStatementNode : public StatementNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
    explicit BlockStatementNode(std::vector<std::unique_ptr<ASTNode>> statements);
    void accept(ASTVisitor &visitor) override;
};


class ExpressionStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> expression;

    explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expression);
    void accept(ASTVisitor &visitor) override;
};

class VarDeclarationNode : public StatementNode {
public:
    bool isConst;
    std::string name;
    std::unique_ptr<ExpressionNode> initializer;
    std::string declaredType;
    std::string inferType;
    std::string type;
    explicit VarDeclarationNode(std::string name, std::unique_ptr<ExpressionNode> initializer, bool isConst);
    void accept(ASTVisitor &visitor) override;
};

class FunctionDeclarationNode : public StatementNode {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<BlockStatementNode> body;
    std::vector<std::string> parametersTypes;
    explicit FunctionDeclarationNode(std::string name, std::vector<std::string> parameters, std::unique_ptr<BlockStatementNode> body, const std::vector<std::string>& parametersTypes);
    void accept(ASTVisitor &visitor) override;
};

class CallFunctionNode : public ExpressionNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
    std::string returnType;
    explicit CallFunctionNode(std::string name, std::vector<std::unique_ptr<ExpressionNode>> arguments);
    void accept(ASTVisitor &visitor) override;
};

class UnaryExprNode : public ExpressionNode {
public:
    std::string op;
    std::unique_ptr<ExpressionNode> operand;
    std::string type;

    explicit UnaryExprNode(std::string op, std::unique_ptr<ExpressionNode> operand);
    void accept(ASTVisitor &visitor) override;
};


class ReturnStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> returnValue;
    std::string returnType;
    explicit ReturnStatementNode(std::unique_ptr<ExpressionNode> returnValue);
    void accept(ASTVisitor &visitor) override;
};

class IfStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> thenBranch;
    std::unique_ptr<StatementNode> elseBranch;

    explicit IfStatementNode(std::unique_ptr<ExpressionNode> condition,
                    std::unique_ptr<BlockStatementNode> thenBranch,
                    std::unique_ptr<StatementNode> elseBranch);
    void accept(ASTVisitor &visitor) override;
};


class WhileStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> body;

    explicit WhileStatementNode(std::unique_ptr<ExpressionNode> condition,
                       std::unique_ptr<BlockStatementNode> body);
    void accept(ASTVisitor &visitor) override;
};

class DoWhileStatementNode : public StatementNode {
  public:
    std::unique_ptr<WhileStatementNode> whileStatement;

    explicit DoWhileStatementNode(std::unique_ptr<WhileStatementNode> whileStatement);
    void accept(ASTVisitor &visitor) override;
};

class PrintlnStatementNode : public StatementNode {
public:
    std::unique_ptr<ExpressionNode> expression;
    explicit PrintlnStatementNode(std::unique_ptr<ExpressionNode> expression);
    void accept(ASTVisitor &visitor) override;
};




#endif // AST_H
