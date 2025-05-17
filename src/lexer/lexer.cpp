//
// Created by home on 28/03/25.
//

#include <utility>
#include "lexer.h"
#include <regex>


std::string Lexer::truncateAfterSecondDot(const std::string &input) {
    const size_t firstDot = input.find('.');
    if (firstDot == std::string::npos) return input;

    const size_t secondDot = input.find('.', firstDot + 1);
    if (secondDot == std::string::npos) return input;

    return input.substr(0, secondDot);
}


bool Lexer::isAtEnd() const {
    return currentIndex >= sourceCode.length();
}

Lexer::Lexer(std::string sourceCode): sourceCode(move(sourceCode)) {
};

char& Lexer::advance() {
    if (sourceCode.length() < currentIndex || sourceCode.empty()) {
        return sourceCode[currentIndex];
    }


    if (sourceCode[currentIndex] == '\n') {
        line++;
        column = 0;
    } else column++;
    return sourceCode[currentIndex++];
}

char& Lexer::peek() {
    if (sourceCode.length() < currentIndex || sourceCode.empty()) {
        return sourceCode[currentIndex];
    }
    return sourceCode[currentIndex];
}


LexerToken Lexer::identifyNumber() {
    std::string value;
    bool isFloat = false;

    if (!isAtEnd() && sourceCode[currentIndex] == '0'
        && (sourceCode[currentIndex+1] == 'x' ||
            sourceCode[currentIndex+1] == 'X' ||
            sourceCode[currentIndex+1] == 'b' ||
            sourceCode[currentIndex+1] == 'B' ||
            sourceCode[currentIndex+1] == 'o' ||
            sourceCode[currentIndex+1] == 'O'))
    {
        char prefix = sourceCode[currentIndex+1];
        value += sourceCode[currentIndex];
        advance();
        value += sourceCode[currentIndex];
        advance();

        auto validDigit = [&](char c) {
            switch (prefix) {
                case 'x': case 'X':
                    return std::isxdigit(c) || c == '_';
                case 'b': case 'B':
                    return (c == '0' || c == '1') || c == '_';
                case 'o': case 'O':
                    return (c >= '0' && c <= '7') || c == '_';
                default:
                    return false;
            }
        };

        while (!isAtEnd() && validDigit(sourceCode[currentIndex])) {
            value += sourceCode[currentIndex];
            advance();
        }
    }
    else {
        while (!isAtEnd()
               && (std::isdigit(sourceCode[currentIndex]) || sourceCode[currentIndex] == '_' || sourceCode[currentIndex] == '.'))
        {
            if (sourceCode[currentIndex] == '.') {
                if (isFloat) break;
                isFloat = true;
            }
            value += sourceCode[currentIndex];
            advance();
        }
    }

    std::string clean;
    clean.reserve(value.size());
    for (char c : value) {
        if (c != '_') clean += c;
    }

    LexerTokenType type;
    if (isFloat) {
        type = LexerTokenType::NUMBER_FLOAT;
        value = truncateAfterSecondDot(value);
    }
    else {
        type = LexerTokenType::NUMBER_INT;
    }

    return LexerToken(type, clean, 0, 0);
}



LexerToken Lexer::identifySymbol() {
    const char first = sourceCode[currentIndex];
    const std::string value(1, first);
    advance();

    try {
        const auto symbol = LexerTokensMap::getTokenByString(value);
        return LexerToken(symbol, value, line, column);
    } catch (...) {
        return LexerToken(LexerTokenType::UNDEFINED, value, line, column);
    }
}

LexerToken Lexer::identifySpecialSymbol() {
    const char first = sourceCode[currentIndex];
    std::string value(1, first);
    advance();

    std::string temp = value;
    temp += sourceCode[currentIndex];

    const auto isDiferentSymbols = temp == LexerTokensMap::getStringByToken(LexerTokenType::BANG_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::GREATER_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::LESS_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::OPTIONAL_CHAIN)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::PLUS_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::MINUS_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::MULTIPLY_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::DIVIDE_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::MODULUS_EQUAL);

    if (sourceCode[currentIndex] == value[0] || isDiferentSymbols ) {
        value += sourceCode[currentIndex];
        advance();
    }


    try {
        const auto symbol = LexerTokensMap::getTokenByString(value);
        return LexerToken(symbol, value, line, column);
    } catch (...) {
        return LexerToken(LexerTokenType::UNDEFINED, value, line, column);
    }
}

LexerToken Lexer::identifyIdentifierKeyword() {
    std::string value;

    while ((!isspace(sourceCode[currentIndex]) && isalpha(sourceCode[currentIndex]) && !isAtEnd()) || peek() == ':') {
        value += sourceCode[currentIndex];
        advance();
    }

    try {
        const auto keyword = LexerTokensMap::getTokenByString(value);

        return LexerToken(keyword, value, line, column);
    } catch (...) {
        return LexerToken(LexerTokenType::IDENTIFIER, value, line, column);
    }
}






LexerToken Lexer::identify() {
    while (isspace(sourceCode[currentIndex]) && !isAtEnd()) advance();
    if (isAtEnd()) return LexerToken(LexerTokenType::EOS, "", line, column);


    if (isalpha(sourceCode[currentIndex]) || sourceCode[currentIndex] == '_') return identifyIdentifierKeyword();


    if (!isalnum(sourceCode[currentIndex])) {
        if (const std::string twoChar = sourceCode.substr(currentIndex, 2); LexerTokensMap::isValidSymbol(twoChar)) {
            return identifySpecialSymbol();
        }
        return identifySymbol();
    }

    if (!isalnum(sourceCode[currentIndex])) return identifySymbol();

    if (isdigit(sourceCode[currentIndex])) return identifyNumber();


    return LexerToken(LexerTokenType::UNDEFINED, "", line, column);
}


std::vector<LexerToken> Lexer::tokenize() {
    std::vector<LexerToken> tokens;
    LexerToken token;
    do {
        token = identify();
        tokens.push_back(token);
    } while (token.type != LexerTokenType::EOS);

    return tokens;
}
