//
// Created by home on 21/04/25.
//

#include <lexer.h>
#include <gtest/gtest.h>

#include "ast.h"
#include <parser.h>


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



TEST(Parser, StringStrIndex) {
    auto lexer = Lexer("\"Olá, mundo\"[2];");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();
    auto indexAccessor = dynamic_cast<IndexAccessExpr*>(ast->children[0].get());

    auto base = dynamic_cast<StringLiteralExpr*>(indexAccessor->base.get());
    std::cout << "base = \"" << base->value << "\"" << std::endl;

    auto idxNode = dynamic_cast<NumberNode*>(indexAccessor->index.get());
    std::cout << "index = " << idxNode->value << std::endl;
}



TEST(ParserExpression, ArrayLiteralMutation) {
    const std::string code = "var x = [1, 4, 5, 6]; x[3] = 10;";
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();

    auto assignment = dynamic_cast<AssignmentIndexExprNode*>(ast->children[1].get());

    ASSERT_EQ(assignment->name, "x");

    auto idx = dynamic_cast<IndexAccessExpr*>(assignment->target.get());
    ASSERT_NE(idx, nullptr);
    auto id = dynamic_cast<IdentifierExprNode*>(idx->base.get());

    ASSERT_NE(id, nullptr);
    ASSERT_EQ(id->name, "x");
}


TEST(Parser, Insert) {
    auto lexer = Lexer("insert!(arr, 2, 10);");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();

    const auto insertNode = dynamic_cast<InsertStatementNode*>(ast->children[0].get());
    const auto value = dynamic_cast<NumberNode*>(insertNode->value.get());

    ASSERT_NE(insertNode, nullptr);
    ASSERT_EQ(value->value, "10");
}



TEST(Parser, Casting) {
    auto lexer = Lexer("4.3 as Int;");
    auto tokens = lexer.tokenize();
    auto parser = Parser(tokens);

    auto ast = parser.parse();

    auto castNode = dynamic_cast<CastingExpressionNode*>(ast->children[0].get());

    ASSERT_EQ(castNode->targetType, "i32");
}


