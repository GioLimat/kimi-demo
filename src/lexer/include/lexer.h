//
// Created by home on 28/03/25.
//

#ifndef LEXER_H
#define LEXER_H

#include "lexer_tokens.h"
#include <vector>
#include <string>

class Lexer {
    size_t line         = 1;
    size_t column       = 1;
    size_t currentIndex = 0;
    std::string sourceCode;

public:
    explicit Lexer(std::string  source);
    LexerToken identify();
    std::vector<LexerToken> tokenize();

private:
    static std::string truncateAfterSecondDot(const std::string& input);

    LexerToken identifyNumber();
    LexerToken identifyIdentifierKeyword();
    LexerToken identifyCharLiteral();
    LexerToken identifyStringLiteral();
    LexerToken identifySymbol();
    LexerToken identifySpecialSymbol();

    static uint32_t readUtf8CodePoint(const std::string &s, size_t& i);

    [[nodiscard]] bool isAtEnd() const;
    [[nodiscard]] char32_t peekCp() const;
    char32_t advanceCp();
};



#endif //LEXER_H
