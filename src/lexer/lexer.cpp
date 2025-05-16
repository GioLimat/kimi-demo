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

void Lexer::advance() {
    if (sourceCode.length() < currentIndex || sourceCode.empty()) {
        return;
    }


    if (sourceCode[currentIndex] == '\n') {
        line++;
        column = 0;
    } else column++;
    currentIndex++;
}


LexerToken Lexer::identifyNumber() {
    std::string value;
    while (isdigit(sourceCode[currentIndex]) || sourceCode[currentIndex] == '.' && !isAtEnd()) {
        value += sourceCode[currentIndex];
        advance();
    }


    const std::regex pattern("[^0-9.]");
    value = regex_replace(value, pattern, "");


    if (value.find('.') != std::string::npos)
        return LexerToken(LexerTokenType::FLOAT, truncateAfterSecondDot(value), line,
                          column);

    return LexerToken(LexerTokenType::INT, value, line, column);
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

    while (!isspace(sourceCode[currentIndex]) && isalpha(sourceCode[currentIndex]) && !isAtEnd()) {
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
