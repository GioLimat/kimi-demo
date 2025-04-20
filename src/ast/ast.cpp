//
// Created by home on 11/04/25.
//

#include "ast.h"

NumberNode::NumberNode(std::string value)
    : value(std::move(value)), type("int") {}

BooleanNode::BooleanNode(const bool value): value(value) {}

IdentifierExprNode::IdentifierExprNode(std::string name)
    : name(std::move(name)) {}

BinaryExprNode::BinaryExprNode(std::string op,
                               std::unique_ptr<ExpressionNode> left,
                               std::unique_ptr<ExpressionNode> right)
    : op(std::move(op)),
      left(std::move(left)),
      right(std::move(right)) {}


AssignmentExprNode::AssignmentExprNode(std::string name, std::unique_ptr<ExpressionNode> value): name(std::move(name)), value(std::move(value)) {}

VarDeclarationNode::VarDeclarationNode(std::string name,
                                       std::unique_ptr<ExpressionNode> initializer, bool isConst)
    : isConst(isConst),
      name(std::move(name)),
      initializer(std::move(initializer)) {}

FunctionDeclarationNode::FunctionDeclarationNode(std::string name,
                                                 std::vector<std::string> parameters,
                                                 std::vector<std::unique_ptr<StatementNode>> body)
    : name(std::move(name)),
      parameters(std::move(parameters)),
      body(std::move(body)) {}

CallFunctionNode::CallFunctionNode(std::string name,
                                   std::vector<std::unique_ptr<ExpressionNode>> arguments)
    : name(std::move(name)),
      arguments(std::move(arguments)) {}


UnaryExprNode::UnaryExprNode(std::string op, std::unique_ptr<ExpressionNode> operand)
        : op(std::move(op)), operand(std::move(operand)) {}


ExpressionStatementNode::ExpressionStatementNode(std::unique_ptr<ExpressionNode> expression)
        : expression(std::move(expression)) {}

BlockStatementNode::BlockStatementNode(std::vector<std::unique_ptr<StatementNode>> statements): statements(std::move(statements)) {}

ReturnStatementNode::ReturnStatementNode(std::unique_ptr<ExpressionNode> returnValue)
        : returnValue(std::move(returnValue)) {}


IfStatementNode::IfStatementNode(std::unique_ptr<ExpressionNode> condition,
                    std::vector<std::unique_ptr<StatementNode>> thenBranch,
                std::unique_ptr<StatementNode> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)),elseBranch(std::move(elseBranch)) {}


WhileStatementNode::WhileStatementNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockStatementNode> body)
        : condition(std::move(condition)), body(std::move(body)) {}


DoWhileStatementNode::DoWhileStatementNode(std::unique_ptr<WhileStatementNode> whileStatement)
        : whileStatement(std::move(whileStatement)) {}


