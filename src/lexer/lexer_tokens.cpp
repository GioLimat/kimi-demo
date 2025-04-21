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
    {LexerTokenType::INT, "Int"},
    {LexerTokenType::FLOAT, "Float"},
    {LexerTokenType::BOOLEAN, "Boolean"},
    {LexerTokenType::TRUE, "true"},
    {LexerTokenType::FALSE, "false"},
    {LexerTokenType::RETURN, "return"},

    {LexerTokenType::R_PAREN, ")"},
    {LexerTokenType::L_PAREN, "("},
    {LexerTokenType::R_BRACE, "}"},
    {LexerTokenType::L_BRACE, "{"},

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

    {LexerTokenType::BANG_EQUAL, "!="},
    {LexerTokenType::EQUAL_EQUAL, "=="},
    {LexerTokenType::LESS, "<"},
    {LexerTokenType::GREATER, ">"},
    {LexerTokenType::LESS_EQUAL, "<="},
    {LexerTokenType::GREATER_EQUAL, ">="},


    {LexerTokenType::AND_AND, "&&"},
    {LexerTokenType::OR_OR, "||"},

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
