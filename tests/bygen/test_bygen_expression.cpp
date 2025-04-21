//
// Created by home on 19/04/25.
//

#include "bygen.h"
#include "bygen_expression.h"
#include "parser_expression.h"
#include "parser.h"
#include "lexer.h"
#include <gtest/gtest.h>

#include "bygen_declaration.h"
#include "parse_declaration.h"


void assert_nodes(const std::vector<std::string>& byte, const std::vector<std::string> &expected) {
    for (int i = 0; i < byte.size(); i++) {
        ASSERT_EQ(byte[i], expected[i]);
    }
}


std::vector<std::string> loadByGenExpression(const std::string& code) {
    Lexer lexer(code);
    const auto tokens = lexer.tokenize();
    ParserExpression parser(tokens);
    auto expr = parser.parseExpression();
    return BygenExpression::generateExpression(expr.get());
}

std::vector<std::string> loadByGenDeclaration(const std::string& code) {
    Lexer lexer(code);
    const auto tokens = lexer.tokenize();
    ParserDeclaration parser(tokens);
    auto expr = parser.parseDeclaration();
    return ByGenDeclaration::generateDeclaration(expr.get());
}



TEST(ByGenExpression, AddOperation) {
    const auto expr = loadByGenExpression("2 + 4");

    const std::vector<std::string> expected = {"CONST_I32 2", "CONST_I32 4", "ADD_I32"};

    assert_nodes(expr, expected);
}

TEST(ByGenExpression, OperationWithIdentAndNumbers) {
    const auto expr = loadByGenExpression("2 + x * 4 / 2");

    const std::vector<std::string> expected = {
        "CONST_I32 2",
        "LOAD x",
        "CONST_I32 4",
        "MUL_I32",
        "CONST_I32 2",
        "DIV_I32",
        "ADD_I32"
    };


    assert_nodes(expr, expected);
}

TEST(ByGenExpression, OperationWithIdentAndNumbersParen) {
    const auto expr = loadByGenExpression("(2 + x) * 4 / 2");


    const std::vector<std::string> expected = {
        "CONST_I32 2",
        "LOAD x",
        "ADD_I32",
        "CONST_I32 4",
        "MUL_I32",
        "CONST_I32 2",
        "DIV_I32"
    };

    assert_nodes(expr, expected);
}


TEST(ByGenDeclarations, VarDeclaration) {
    const auto decla = loadByGenDeclaration("var x = 21 + 4 * 4;");

    const std::vector<std::string> expected = {
        "CONST_I32 21",
        "CONST_I32 4",
        "CONST_I32 4",
        "MUL_I32",
        "ADD_I32",
        "STORE x"
    };


    assert_nodes(decla, expected);
}