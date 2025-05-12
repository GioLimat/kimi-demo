
 /*
 Created by home on 11/04/25.
 */


#include "ast.h"
#include "visitor.h"



NumberNode::NumberNode(std::string value)
    : value(std::move(value)) {}

NumberNode::NumberNode(std::string value, std::string type)
    : value(std::move(value)), type(std::move(type)) {}

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
                                                std::vector<Param> parameters,
                                                 std::unique_ptr<BlockStatementNode> body
                                                )
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

BlockStatementNode::BlockStatementNode(std::vector<std::unique_ptr<ASTNode>> statements): statements(std::move(statements)) {}

ReturnStatementNode::ReturnStatementNode(std::unique_ptr<ExpressionNode> returnValue)
        : returnValue(std::move(returnValue)) {}


IfStatementNode::IfStatementNode(std::unique_ptr<ExpressionNode> condition,
                    std::unique_ptr<BlockStatementNode> thenBranch,
                std::unique_ptr<StatementNode> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)),elseBranch(std::move(elseBranch)) {}


WhileStatementNode::WhileStatementNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockStatementNode> body)
        : condition(std::move(condition)), body(std::move(body)) {}


DoWhileStatementNode::DoWhileStatementNode(std::unique_ptr<WhileStatementNode> whileStatement)
        : whileStatement(std::move(whileStatement)) {}


PrintlnStatementNode::PrintlnStatementNode(std::unique_ptr<ExpressionNode> expression) : expression(std::move(expression)) {}


GenericExpressionNode::GenericExpressionNode(std::unique_ptr<ExpressionNode> node) : node(std::move(node)) {}


PostFixExprNode::PostFixExprNode(std::string op, std::unique_ptr<ExpressionNode> operand) : op(std::move(op)), operand(std::move(operand)) {}





 //VISITORS

void NumberNode::accept(ASTVisitor &visitor) {
        visitor.visitNumber(this);
}

void BooleanNode::accept(ASTVisitor &visitor) {
        visitor.visitBoolean(this);
}

void IdentifierExprNode::accept(ASTVisitor &visitor) {
        visitor.visitIdentifier(this);
}

void BinaryExprNode::accept(ASTVisitor &visitor) {
        visitor.visitBinaryExpr(this);
}

void AssignmentExprNode::accept(ASTVisitor &visitor) {
        visitor.visitAssignmentExpr(this);
}

void VarDeclarationNode::accept(ASTVisitor &visitor) {
        visitor.visitVarDeclaration(this);
}

void FunctionDeclarationNode::accept(ASTVisitor &visitor) {
        visitor.visitFunctionDeclaration(this);
}

void CallFunctionNode::accept(ASTVisitor &visitor) {
        visitor.visitCallFunction(this);
}

void UnaryExprNode::accept(ASTVisitor &visitor) {
        visitor.visitUnaryExpr(this);
}

void ExpressionStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitExpressionStatement(this);
}

void BlockStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitBlockStatement(this);
}

void ReturnStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitReturnStatement(this);
}

void IfStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitIfStatement(this);
}

void WhileStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitWhileStatement(this);
}

void DoWhileStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitDoWhileStatement(this);
}

void PrintlnStatementNode::accept(ASTVisitor &visitor) {
        visitor.visitPrintln(this);
}


void StatementNode::accept(ASTVisitor &visitor) {
        visitor.visitStatementNode(this);
}

void ExpressionNode::accept(ASTVisitor &visitor) {
        visitor.visitExpressionNode(this);
}

void GenericExpressionNode::accept(ASTVisitor &visitor) {
        visitor.visitGenericExpressionNode(this);
}


void PostFixExprNode::accept(ASTVisitor &visitor) {
        visitor.visitPostFixExpr(this);
}