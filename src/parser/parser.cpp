//
// Created by home on 11/04/25.
//
#include "parser.h"

#include <parser_expression.h>


Parser::Parser(const std::vector<LexerToken>& tokens) : tokens(tokens) {}

std::vector<LexerToken> Parser::tokensByCurrentBlock(int& blockEnd_)  {
    const int blockStart = static_cast<int>(current);
    const int blockEnd = findMatchingBrace(blockStart);
    if (blockEnd == -1) {
        throw std::runtime_error("Expected '}' at end of block body");
    }
    const int bodyLength = blockEnd - blockStart + 1;

    const auto tokensS = tokens
        | std::views::drop(blockStart)
        | std::views::take(bodyLength);
    const std::vector sliced(tokensS.begin(), tokensS.end());
    blockEnd_  = blockEnd - blockStart;
    return sliced;
}


int Parser::findEndOfExpression(const size_t start) const {

    for (size_t i = start; i < tokens.size(); ++i) {
        if (tokens[i].type == LexerTokenType::SEMICOLON || tokens[i].type == LexerTokenType::R_BRACE) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int Parser::findEndOfParenBlock(const size_t start) const {
    if (start >= tokens.size() || tokens.at(start).type != LexerTokenType::L_PAREN) {
        throw std::runtime_error("Expected '(' at start of block for parenthesis matching.");
    }

    int depth = 0;
    for (size_t i = start; i < tokens.size(); ++i) {
        if (tokens[i].type == LexerTokenType::L_PAREN) {
            depth++;
        } else if (tokens[i].type == LexerTokenType::R_PAREN) {
            depth--;
            if (depth == 0) {
                return static_cast<int>(i + 1);
            }
        }
    }

    throw std::runtime_error("No matching ')' found for '('.");
}

int Parser::findMatchingBrace(int start) const {
    if (start >= static_cast<int>(tokens.size()) || tokens.at(start).type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' at start of block for brace matching.");
    }

    int depth = 0;
    for (size_t i = start; i < tokens.size(); ++i) {
        if (tokens[i].type == LexerTokenType::L_BRACE) {
            depth++;
        } else if (tokens[i].type == LexerTokenType::R_BRACE) {
            depth--;
            if (depth == 0) {
                return static_cast<int>(i);
            }
        }
    }

    throw std::runtime_error("No matching '}' found for '{'.");
}



int Parser::findIndex(const size_t start, const LexerTokenType token) const {
    for (size_t i = start; i < tokens.size(); ++i) {
        if (tokens[i].type == token) {
            return static_cast<int>(i);
        }
    }
    return -1;
}


std::unique_ptr<ExpressionNode> Parser::delegateToExpression(size_t i) {
    const auto index = i == -1 ? tokens.size() - 1 : i;
    const auto sliced = slice(index);
    auto exprParser = ParserExpression(sliced);
    auto expr = exprParser.parseExpression();
    current += exprParser.current;
    return std::move(expr);
}


std::vector<LexerToken> Parser::slice(size_t i) {
    const auto tokensS = tokens | std::views::drop(current) | std::views::take(i);
    const std::vector sliced(tokensS.begin(), tokensS.end());
    return sliced;
}

LexerToken Parser::peek() const {
    if (isAtEnd()) {
        return  tokens.at(tokens.size() - 1);
    }
    return tokens.at(current);
}

std::optional<LexerToken> Parser::previous() const {
    if (current == 0 || current > tokens.size()) return std::nullopt;
    return tokens[current - 1];
}

LexerToken Parser::advance() {
    if (!isAtEnd()) {
        return tokens.at(current++);
    }

    return tokens.at(tokens.size() - 1);
}

bool Parser::isAtEnd() const {
    return tokens[current].type == LexerTokenType::EOS;
}