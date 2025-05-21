//
// Created by home on 28/03/25.
//

#include <utility>
#include "lexer.h"

#include <codecvt>
#include <regex>


std::string Lexer::truncateAfterSecondDot(const std::string &input) {
    const size_t firstDot = input.find('.');
    if (firstDot == std::string::npos) return input;

    const size_t secondDot = input.find('.', firstDot + 1);
    if (secondDot == std::string::npos) return input;

    return input.substr(0, secondDot);
}

uint32_t Lexer::readUtf8CodePoint(const std::string& s, size_t& i) {
    const auto c0 = static_cast<unsigned char>(s[i]);
    if (c0 < 0x80) {
        ++i;
        return c0;
    }
    int width = 0;
    if    ((c0 & 0xE0) == 0xC0) width = 2;
    else if ((c0 & 0xF0) == 0xE0) width = 3;
    else if ((c0 & 0xF8) == 0xF0) width = 4;
    else throw std::runtime_error("Invalid UTF-8 start byte");
    uint32_t cp = c0 & ((1 << (8 - width)) - 1);
    for (int k = 1; k < width; ++k) {
        const auto cx = static_cast<unsigned char>(s[i+k]);
        if ((cx & 0xC0) != 0x80) throw std::runtime_error("Invalid UTF-8 continuation byte");
        cp = (cp << 6) | (cx & 0x3F);
    }
    i += width;
    return cp;
}



bool Lexer::isAtEnd() const {
    return currentIndex >= sourceCode.length();
}

Lexer::Lexer(std::string  source): sourceCode(std::move(source)) {
};

char32_t Lexer::peekCp() const {
    if (isAtEnd()) return U'\0';
    size_t tmp = currentIndex;
    return static_cast<char32_t>(readUtf8CodePoint(sourceCode, tmp));
}

char32_t Lexer::advanceCp() {
    const auto cp = static_cast<char32_t>(readUtf8CodePoint(sourceCode, currentIndex));
    if (cp == U'\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return cp;
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
        advanceCp();
        advanceCp();

        std::string rawDigits;
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
            if (sourceCode[currentIndex] != '_')
                rawDigits += sourceCode[currentIndex];
            advanceCp();
        }
        int base = (prefix == 'x' || prefix == 'X') ? 16 :
                   (prefix == 'b' || prefix == 'B') ? 2 :
                   (prefix == 'o' || prefix == 'O') ? 8 : 10;

        int64_t number = std::stoll(rawDigits, nullptr, base);
        value = std::to_string(number);
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
            advanceCp();
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
    advanceCp();

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
    advanceCp();

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
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::MODULUS_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::BIT_AND_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::BIT_OR_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::XOR_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::SHIFT_LEFT_EQUAL)
                                        || temp == LexerTokensMap::getStringByToken(LexerTokenType::SHIFT_RIGHT_EQUAL);

    if (peekCp() == value[0] || isDiferentSymbols ) {
        value += peekCp();
        advanceCp();
    }


    try {
        const auto symbol = LexerTokensMap::getTokenByString(value);
        return LexerToken(symbol, value, line, column);
    } catch (...) {
        return LexerToken(LexerTokenType::UNDEFINED, value, line, column);
    }
}

LexerToken Lexer::identifyIdentifierKeyword() {
    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> conv;
    std::string utf8;
    while (!isAtEnd()) {
        const char32_t cp = peekCp();
        if (const bool isLetter = (cp < 128 ? std::isalpha(cp) : true); !isLetter && cp != U'_' && !(cp >= U'0' && cp <= U'9')) break;
        size_t tmp = currentIndex;
        const uint32_t cp2 = readUtf8CodePoint(sourceCode, tmp);
        utf8 += conv.to_bytes(cp2);
        advanceCp();
    }
    try {
        auto tk = LexerTokensMap::getTokenByString(utf8);
        return LexerToken(tk, utf8, line, column);
    } catch(...) {
        return LexerToken(LexerTokenType::IDENTIFIER, utf8, line, column);
    }
}






LexerToken Lexer::identify() {
    while (!isAtEnd() && std::isspace(static_cast<unsigned char>(peekCp())))
        advanceCp();
    if (isAtEnd())
        return LexerToken(LexerTokenType::EOS, "", line, column);

    if (peekCp() == U'\'') {
        advanceCp();
        if (isAtEnd()) throw std::runtime_error("Unterminated char");
        char32_t cp = advanceCp();
        if (peekCp() != U'\'') throw std::runtime_error("Unterminated char");
        advanceCp();
        std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
        return LexerToken(LexerTokenType::CHAR_LITERAL, cv.to_bytes(cp), line, column);
    }

    char32_t cp = peekCp();
    if ((cp < 128 && std::isalpha(cp)) || cp == U'_')
        return identifyIdentifierKeyword();
    if (cp >= U'0' && cp <= U'9')
        return identifyNumber();

    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;

    std::string two = cv.to_bytes(peekCp());

    size_t tmp = currentIndex;

    readUtf8CodePoint(sourceCode, tmp);

    two += cv.to_bytes(readUtf8CodePoint(sourceCode, tmp));

    if (LexerTokensMap::isValidSymbol(two))
        return identifySpecialSymbol();

    return identifySymbol();
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
