//
// Created by home on 28/03/25.
//
#include "include/lexer_tokens.h"

const std::unordered_map<LexerTokenType, std::string> LexerTokensMap::lexerTokensMap = {
    {LexerTokenType::PRINTLN, "println"},


    {LexerTokenType::VAR, "var"},
    {LexerTokenType::VAL, "val"},
    {LexerTokenType::IDENTIFIER, "IDENTIFIER"},
    {LexerTokenType::FN, "fn"},
    {LexerTokenType::IF, "if"},
    {LexerTokenType::ELSE, "else"},
    {LexerTokenType::WHILE, "while"},
    {LexerTokenType::FOR, "for"},
    {LexerTokenType::DO, "do"},

    {LexerTokenType::NUMBER_INT, "NUMBER_FLOAT"},
    {LexerTokenType::NUMBER_FLOAT, "NUMBER_INT"},
    {LexerTokenType::INT, "Int"},
    {LexerTokenType::FLOAT, "Float"},
    {LexerTokenType::BOOLEAN, "Bool"},
    {LexerTokenType::TRUE, "true"},
    {LexerTokenType::FALSE, "false"},

    {LexerTokenType::RETURN, "return"},

    {LexerTokenType::R_PAREN, ")"},
    {LexerTokenType::L_PAREN, "("},
    {LexerTokenType::R_BRACE, "}"},
    {LexerTokenType::L_BRACE, "{"},
    {LexerTokenType::R_BRACKET, "]"},
    {LexerTokenType::L_BRACKET, "["},


    {LexerTokenType::COMMA, ","},
    {LexerTokenType::SEMICOLON, ";"},
    {LexerTokenType::COLON, ":"},

    {LexerTokenType::BANG, "!"},
    {LexerTokenType::PLUS, "+"},
    {LexerTokenType::MINUS, "-"},
    {LexerTokenType::MULTIPLY, "*"},
    {LexerTokenType::DIVIDE, "/"},
    {LexerTokenType::MODULUS, "%"},
    {LexerTokenType::EQUALS, "="},
    {LexerTokenType::OPTIONAL_CHAIN, "?."},
    {LexerTokenType::PLUS_PLUS, "++"},
    {LexerTokenType::MINUS_MINUS, "--"},
    {LexerTokenType::BIT_AND, "&"},
    {LexerTokenType::BIT_OR, "|"},
    {LexerTokenType::XOR, "^"},
    {LexerTokenType::BIT_NOT, "~"},
    {LexerTokenType::SHIFT_LEFT, "<<"},
    {LexerTokenType::SHIFT_RIGHT, ">>"},




    {LexerTokenType::BANG_EQUAL, "!="},
    {LexerTokenType::EQUAL_EQUAL, "=="},
    {LexerTokenType::LESS, "<"},
    {LexerTokenType::GREATER, ">"},
    {LexerTokenType::LESS_EQUAL, "<="},
    {LexerTokenType::GREATER_EQUAL, ">="},
    {LexerTokenType::BIT_AND_EQUAL, "&="},
    {LexerTokenType::BIT_OR_EQUAL, "|="},
    {LexerTokenType::XOR_EQUAL, "^="},
    {LexerTokenType::SHIFT_LEFT_EQUAL, "<<="},
    {LexerTokenType::SHIFT_RIGHT_EQUAL, ">>="},

    {LexerTokenType::PLUS_EQUAL, "+="},
    {LexerTokenType::MINUS_EQUAL, "-="},
    {LexerTokenType::MULTIPLY_EQUAL, "*="},
    {LexerTokenType::DIVIDE_EQUAL, "/="},
    {LexerTokenType::MODULUS_EQUAL, "%="},


    {LexerTokenType::AND_AND, "&&"},
    {LexerTokenType::OR_OR, "||"},

    {LexerTokenType::CHAR_LITERAL, "CHAR_LITERAL"},
    {LexerTokenType::CHAR, "Char"},

    {LexerTokenType::EOS, "EOS"},
    {LexerTokenType::UNDEFINED, "UNDEFINED"}
};


std::unordered_map<std::string, LexerTokenType> LexerTokensMap::invertedTokensMap = LexerTokensMap::invertTokensMap();

bool LexerTokensMap::tokenIs(const LexerTokenType type) {
    return lexerTokensMap.contains(type);
}

LexerTokenType LexerTokensMap::getTokenByString(const std::string &tokenStr) {
    if (const auto it = invertedTokensMap.find(tokenStr); it != invertedTokensMap.end()) return it->second;
    throw std::invalid_argument("LexerTokensMap::getTokenByString: invalid token std::string");
}

std::string LexerTokensMap::getStringByToken(const LexerTokenType tokenType) {
    if (const auto it = lexerTokensMap.find(tokenType); it != lexerTokensMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("LexerTokensMap::getStringByToken: invalid token type");
}

std::unordered_map<std::string, LexerTokenType> LexerTokensMap::invertTokensMap() {
    std::unordered_map<std::string, LexerTokenType> invertedMap;
    for (const auto &[fst, snd]: lexerTokensMap) {
        invertedMap[snd] = fst;
    }
    return invertedMap;
}


bool LexerTokensMap::isValidSymbol(const std::string& symbol) {
    try {
       getTokenByString(symbol);
        return true;
    } catch (...) {
        return false;
    }
}
