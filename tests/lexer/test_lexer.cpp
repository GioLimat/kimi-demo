//
// Created by home on 29/03/25.
//
#include <gtest/gtest.h>
#include "../../src/lexer/include/lexer.h"

void assertTokenVector(const std::vector<LexerToken>& actual,
                       const std::vector<LexerTokenType>& expectedTypes) {
    ASSERT_EQ(actual.size(), expectedTypes.size());
    for (size_t i = 0; i < actual.size(); ++i) {
        ASSERT_EQ(actual[i].type, expectedTypes[i]);
    }
}

TEST(LexerTest, SimpleVariableAsignment) {
    auto lexer = Lexer("val x = 1");

    const std::vector<LexerToken> tokens = lexer.tokenize();

    const std::vector<LexerTokenType> expected = {
        LexerTokenType::VAL,
        LexerTokenType::IDENTIFIER,
        LexerTokenType::EQUALS,
        LexerTokenType::INT,
        LexerTokenType::EOS
    };


    assertTokenVector(tokens, expected);
}


TEST(LexerTest, SimpleFunctionWithTokenize) {
    auto lexer = Lexer("fn add(x, y){x+y}");

    const std::vector<LexerToken> tokens = lexer.tokenize();

    const std::vector expected = {
            LexerTokenType::FN,
            LexerTokenType::IDENTIFIER,
            LexerTokenType::L_PAREN,
            LexerTokenType::IDENTIFIER,
            LexerTokenType::COMMA,
            LexerTokenType::IDENTIFIER,
            LexerTokenType::R_PAREN,
            LexerTokenType::L_BRACE,
            LexerTokenType::IDENTIFIER,
            LexerTokenType::PLUS,
            LexerTokenType::IDENTIFIER,
            LexerTokenType::R_BRACE,
            LexerTokenType::EOS
    };

    assertTokenVector(tokens, expected);
}


TEST(LexerTest, FunctionEmpty) {
    auto lexer = Lexer("fn add(){}");
    const std::vector<LexerToken> tokens = lexer.tokenize();

    const std::vector expected = {
        LexerTokenType::FN,
        LexerTokenType::IDENTIFIER,
        LexerTokenType::L_PAREN,
        LexerTokenType::R_PAREN,
        LexerTokenType::L_BRACE,
        LexerTokenType::R_BRACE,
        LexerTokenType::EOS
};

    assertTokenVector(tokens, expected);
}

TEST(LexerTest, BooleanExpression) {
    auto lexer = Lexer("true == false");
    const auto tokens = lexer.tokenize();

    const auto expected = {
        LexerTokenType::TRUE,
        LexerTokenType::EQUAL_EQUAL,
        LexerTokenType::FALSE,
        LexerTokenType::EOS
    };

    assertTokenVector(tokens, expected);
}