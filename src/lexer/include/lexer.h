//
// Created by home on 28/03/25.
//

#ifndef LEXER_H
#define LEXER_H

#include "lexer_tokens.h"
#include <vector>
#include <string>

class Lexer {
    size_t line = 0;
    size_t column = 0;
    size_t currentIndex = 0;
    std::string sourceCode;

    public:
        explicit Lexer(std::string sourceCode);
        LexerToken identify();
        std::vector<LexerToken> tokenize();

    private:
        static std::string truncateAfterSecondDot(const std::string& input);
        LexerToken identifyNumber();
        LexerToken identifyIdentifierKeyword();
        [[nodiscard]] bool isAtEnd() const;
        LexerToken identifySymbol();
        LexerToken identifySpecialSymbol();
        char& advance();
        char& peek();

};


#endif //LEXER_H
