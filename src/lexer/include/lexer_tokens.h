//
// Created by home on 28/03/25.
//

#ifndef LEXER_TOKENS_H
#define LEXER_TOKENS_H

#include <unordered_map>
#include <string>
#include <iostream>




enum class LexerTokenType {

    // Macros
    PRINTLN,

    // Keywords
    VAR, VAL, FN, IF, ELSE, WHILE, FOR, DO, RETURN,

    // Identifiers and literals
    IDENTIFIER, INT, FLOAT, BOOLEAN, TRUE, FALSE,

    // Symbols and punctuation
    L_PAREN, R_PAREN,
    L_BRACE, R_BRACE,
    COMMA, SEMICOLON, COLON,

    // Unary and binary operators
    BANG,       // !
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    MODULUS,    // %
    EQUALS,     // =
    OPTIONAL_CHAIN,         // ?.
    PLUS_PLUS,   // ++
    MINUS_MINUS,  // --

    // Comparison operators
    EQUAL_EQUAL,      // ==
    BANG_EQUAL,       // !=
    LESS,             // <
    GREATER,          // >
    LESS_EQUAL,       // <=
    GREATER_EQUAL,    // >=
    PLUS_EQUAL,      // +=


    // Logical operators
    AND_AND,          // &&
    OR_OR,            // ||

    // Misc
    EOS,              // End of stream
    UNDEFINED         // Invalid or unknown token
};



struct LexerToken {
    LexerTokenType type;
    std::string value;
    size_t line;
    size_t column;
};

class LexerTokensMap {
    static const std::unordered_map<LexerTokenType, std::string> lexerTokensMap;
    static std::unordered_map<std::string, LexerTokenType> invertedTokensMap;
    public:
        static bool tokenIs(LexerTokenType type);
        static LexerTokenType getTokenByString(const std::string& tokenStr);
        static std::unordered_map<std::string, LexerTokenType> invertTokensMap();
        static std::string getStringByToken(LexerTokenType tokenType);
        static bool isValidSymbol(const std::string& symbol);
};

#endif // LEXER_TOKENS_H
