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
    auto lexer = Lexer("if (x > 4) { var y = 5; }");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();

}



TEST(Parser, DoWhile) {
    auto lexer = Lexer("do { println(4); } while (6 > 4)");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();
}


TEST(Parser, ForLoop) {
    auto lexer = Lexer("for (var i = 0; i < 10; i++) { println(i); } val x = 4;");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();
}




