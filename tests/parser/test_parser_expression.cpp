//
// Created by home on 11/04/25.
//


#include <gtest/gtest.h>

#include <cmath>
#include "lexer.h"
#include "parser_expression.h"
#include "ast.h"


class MockExpression : public ParserExpression {
    public:
        explicit MockExpression(const std::vector<LexerToken>& tokens) : ParserExpression(tokens) {}


        std::unique_ptr<ExpressionNode> parsePrimaryMock()  {
        return parsePrimary();
    }
};


double evaluateExpression(const ExpressionNode* node) {
    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        return std::stod(num->value);
    }

    if (auto bin = dynamic_cast<const BinaryExprNode*>(node)) {
        double left = evaluateExpression(bin->left.get());
        double right = evaluateExpression(bin->right.get());

        if (bin->op == "+") return left + right;
        if (bin->op == "-") return left - right;
        if (bin->op == "*") return left * right;
        if (bin->op == "/") return right != 0 ? left / right : throw std::runtime_error("Division by zero");
        if (bin->op == "%") return std::fmod(left, right); 
        throw std::runtime_error("Unknown operator: " + bin->op);
    }

    throw std::runtime_error("Unknown expression node type");
}


TEST(ParserExpression, SimpleExpression) {
    const std::string code = "1 + 2 * 5;";
    Lexer lexer(code);

    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    const std::unique_ptr<ExpressionNode> expr = parser.parseExpression();

    const double result = evaluateExpression(expr.get());

    EXPECT_EQ(result, 11);
}

TEST(ParserExpression, PrecedenceWithPharensis) {
    const std::string code = "5 * ((1 + 2) / (1 + 2))";

    Lexer lexer(code);
    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    const std::unique_ptr<ExpressionNode> expr = parser.parseExpression();


    const double result = evaluateExpression(expr.get());


    ASSERT_EQ(result, 5);
}

TEST(ParserExpression, ComplexExpression) {
    const std::string code = "1 * 2 + 2 - 5 + 6 / 3 + (3 + 6) * 2 + (4 % 2)";

    Lexer lexer(code);
    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    std::unique_ptr<ExpressionNode> expr = parser.parseExpression();

    EXPECT_EQ(evaluateExpression(expr.get()), 19);
}


TEST(ParserExpression, SingleNumber) {
    const std::string code = "1";

    Lexer lexer(code);
    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    std::unique_ptr<ExpressionNode> expr = parser.parseExpression();

    EXPECT_EQ(evaluateExpression(expr.get()), 1);
}

TEST(ParserExpression, FunctionCall) {
    const std::string code = "add(1, 2 + 3)";
    Lexer lexer(code);
    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    std::unique_ptr<ExpressionNode> expr = parser.parseExpression();

    auto callFunc = dynamic_cast<CallFunctionNode*>(expr.get());

    ASSERT_EQ(callFunc->name, "add");
    ASSERT_EQ(callFunc->arguments.size(), 2);

    auto arg1 = dynamic_cast<NumberNode*>(callFunc->arguments[0].get());
    ASSERT_EQ(arg1->value, "1");

    auto arg2 = dynamic_cast<BinaryExprNode*>(callFunc->arguments[1].get());
    ASSERT_EQ(arg2->op, "+");

    auto left = dynamic_cast<NumberNode*>(arg2->left.get());
    auto right = dynamic_cast<NumberNode*>(arg2->right.get());

    ASSERT_EQ(left->value, "2");
    ASSERT_EQ(right->value, "3");
    ASSERT_EQ(evaluateExpression(arg2), 5);
}



TEST(ParserExpression, UnaryExpression) {
    const std::string code = "-a";
    Lexer lexer(code);

    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    auto expr = parser.parseExpression();

    auto unary = dynamic_cast<UnaryExprNode*>(expr.get());

    ASSERT_EQ(unary->op, "-");

    auto id = dynamic_cast<IdentifierExprNode*>(unary->operand.get());
    ASSERT_NE(id, nullptr);
    ASSERT_EQ(id->name, "a");
}




TEST(ParserExpression, BooleanExpression) {
    const std::string code = "true == false";
    Lexer lexer(code);

    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    auto expr = parser.parseExpression();

    auto bin = dynamic_cast<BinaryExprNode*>(expr.get());

    ASSERT_EQ(bin->op, "==");

    auto trueNode = dynamic_cast<BooleanNode*>(bin->left.get());
    ASSERT_EQ(trueNode->value, true);
    auto falseNode = dynamic_cast<BooleanNode*>(bin->right.get());
    ASSERT_EQ(falseNode->value, false);
};


TEST(ParserExpression, AssignmentExpression) {
    const std::string code = "x = 4;";
    Lexer lexer(code);

    const auto tokens = lexer.tokenize();

    auto parser = MockExpression(tokens);

    auto expr = parser.parseExpression();

    auto assignment = dynamic_cast<AssignmentExprNode*>(expr.get());
    auto exprAsg = dynamic_cast<NumberNode*>(assignment->value.get());


    ASSERT_EQ(exprAsg->value, "4");
    ASSERT_EQ(assignment->name, "x");
}


