//
// Created by home on 21/04/25.
//

#include <lexer.h>
#include <gtest/gtest.h>
#include "parser.h"


TEST(Parser, FullParser) {
    auto lexer = Lexer("var x = 4;");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();


    ASSERT_EQ(ast.get()->children.size(), 1);

    auto varDeclaration = dynamic_cast<VarDeclarationNode*>(ast->children[0].get());

    ASSERT_EQ(varDeclaration->name, "x");

    auto init = dynamic_cast<NumberNode*>(varDeclaration->initializer.get());

    ASSERT_EQ(init->value, "4");
}


TEST(Parser, IfParser) {
    auto lexer = Lexer("if (x > 4){x+4;}");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();

    ASSERT_EQ(ast.get()->children.size(), 1);

    auto ifStatement = dynamic_cast<IfStatementNode*>(ast->children[0].get());

    auto condition = dynamic_cast<BinaryExprNode*>(ifStatement->condition.get());
    auto v1 = dynamic_cast<IdentifierExprNode*>(condition->left.get());
    auto v2 = dynamic_cast<NumberNode*>(condition->right.get());

    ASSERT_EQ(v1->name, "x");
    ASSERT_EQ(v2->value, "4");

}