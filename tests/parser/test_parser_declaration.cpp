//
// Created by home on 15/04/25.
//

#include <cmath>

#include "parse_declaration.h"
#include "lexer.h"
#include <gtest/gtest.h>


std::unique_ptr<StatementNode> getStatementByDeclaration(const std::string &code) {
    auto lexer = Lexer(code);

    const auto tokens = lexer.tokenize();


    auto parser = ParserDeclaration(tokens);

    return  parser.parseDeclaration();
}

std::unique_ptr<StatementNode> getStatementByStatement(const std::string &code) {
    auto lexer = Lexer(code);

    const auto tokens = lexer.tokenize();

    auto parser = ParserStatement(tokens);

    return  parser.parseStatement();
}



TEST(ParserDeclaration, VarDeclaration) {

    const std::unique_ptr<StatementNode> statement = getStatementByDeclaration("var x = 5;");

    auto varDecl = dynamic_cast<VarDeclarationNode*>(statement.get());
    auto number = dynamic_cast<NumberNode*>(varDecl->initializer.get());


    ASSERT_EQ(varDecl->name, "x");
    ASSERT_EQ(varDecl->isConst, false);
    ASSERT_EQ(number->value, "5");
}


TEST(ParserDeclaration, FnDeclarationWithReturnImplicit) {
    const std::unique_ptr<StatementNode> statement = getStatementByDeclaration("fn sum(a: Int, b: Int){ val x = 10; 4 + 4 }");


    auto fnDeclaration = dynamic_cast<FunctionDeclarationNode*>(statement.get());
    ASSERT_EQ(fnDeclaration->name, "sum");
    ASSERT_EQ(fnDeclaration->parameters[0].type, "i32");
    ASSERT_EQ(fnDeclaration->body.get()->statements.size(), 2);
}



TEST(ParserDeclaration, FnDeclarationMulti) {
    const std::unique_ptr<StatementNode> statement = getStatementByDeclaration("fn sum(a: Int, b: Int) {return a+b; b+a } ");


    auto fnDeclaration = dynamic_cast<FunctionDeclarationNode*>(statement.get());
    auto st1 = dynamic_cast<ReturnStatementNode*>(fnDeclaration->body->statements[0].get());
    auto expr = dynamic_cast<BinaryExprNode*>(st1->returnValue.get());
    auto v1 = dynamic_cast<IdentifierExprNode*>(expr->left.get());
    auto v2 = dynamic_cast<IdentifierExprNode*>(expr->right.get());

    ASSERT_EQ(fnDeclaration->body.get()->statements.size(), 2);
    ASSERT_EQ(v1->name, "a");
    ASSERT_EQ(v2->name, "b");
}


TEST(ParserStatement, IfStatement) {
    const auto statement = getStatementByStatement("if(10 > 40){ 10 + 20; }");

    auto ifStatement = dynamic_cast<IfStatementNode*>(statement.get());
    auto expr = dynamic_cast<BinaryExprNode*>(ifStatement->condition.get());

    ASSERT_EQ(expr->op, ">");
    auto n1 = dynamic_cast<NumberNode*>(expr->left.get());
    auto n2 = dynamic_cast<NumberNode*>(expr->right.get());

    ASSERT_EQ(n1->value, "10");
    ASSERT_EQ(n2->value, "40");
}


TEST(ParserStatement, IfElseMultipleStatements) {
    const auto statement = getStatementByStatement(
        "if (x < 5) { x + 10; 30+ 4; } else { 4 + 20; } "
    );

    auto ifStatement = dynamic_cast<IfStatementNode*>(statement.get());
    ASSERT_NE(ifStatement->elseBranch.get(), nullptr);
}


TEST(ParserExpression, WhileStatement) {
    const auto statement = getStatementByStatement(
        "while(x < 5) { x + 10 }"
    );

    auto whileStatement = dynamic_cast<WhileStatementNode*>(statement.get());

    auto condition = dynamic_cast<BinaryExprNode*>(whileStatement->condition.get());
    auto n1 = dynamic_cast<IdentifierExprNode*>(condition->left.get());
    auto n2 = dynamic_cast<NumberNode*>(condition->right.get());

    auto body = whileStatement->body.get();


    ASSERT_EQ(body->statements.size(), 1);
    ASSERT_EQ(n1->name, "x");
    ASSERT_EQ(n2->value, "5");
    ASSERT_EQ(condition->op, "<");
}

TEST(ParserExpression, DoWhileStatement) {
    const auto statement = getStatementByStatement(
        "do{ x + 10 }while(x > 4)"
    );

    auto doWhileStatement = dynamic_cast<DoWhileStatementNode*>(statement.get());
    auto whileStatement = dynamic_cast<WhileStatementNode*>(doWhileStatement->whileStatement.get());
    auto condition = dynamic_cast<BinaryExprNode*>(whileStatement->condition.get());
    auto body = whileStatement->body.get();
    ASSERT_EQ(body->statements.size(), 1);

    ASSERT_EQ(condition->op, ">");
}


TEST(ParserStatement, PrintStatement) {
    const auto statement = getStatementByStatement("println(10) ");

    auto printStatement = dynamic_cast<PrintlnStatementNode*>(statement.get());

    auto n1 = dynamic_cast<NumberNode*>(printStatement->expression.get());

    ASSERT_EQ(n1->value, "10");
}