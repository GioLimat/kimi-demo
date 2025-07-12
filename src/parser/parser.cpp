//
// Created by home on 11/04/25.
//
#include "parser.h"

#include <parser_expression.h>
#include <parser_statement.h>
#include <parse_declaration.h>


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
    blockEnd_  = blockEnd;
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

int Parser::findEndParenDisconsideredFirst(size_t start) const {
    size_t i = start;
    while (i < tokens.size() && tokens[i].type != LexerTokenType::L_PAREN) {
        ++i;
    }
    if (i >= tokens.size()) {
        throw std::runtime_error("Expected '(' to start a block.");
    }
    int depth = 1;
    for (size_t j = i + 1; j < tokens.size(); ++j) {
        if (tokens[j].type == LexerTokenType::L_PAREN) {
            ++depth;
        } else if (tokens[j].type == LexerTokenType::R_PAREN) {
            --depth;
            if (depth == 0) {
                return static_cast<int>(j);
            }
        }
    }

    throw std::runtime_error("No matching ')' found for '('.");
}

int Parser::findEndBraceDisconsideredFirst(size_t start) const {
    size_t i = start;
    while (i < tokens.size() && tokens[i].type != LexerTokenType::L_BRACE) {
        ++i;
    }
    if (i >= tokens.size()) {
        throw std::runtime_error("Expected '{' to start a block.");
    }
    int depth = 1;
    for (size_t j = i + 1; j < tokens.size(); ++j) {
        if (tokens[j].type == LexerTokenType::L_BRACE) {
            ++depth;
        } else if (tokens[j].type == LexerTokenType::R_BRACE) {
            --depth;
            if (depth == 0) {
                return static_cast<int>(j);
            }
        }
    }

    throw std::runtime_error("No matching '}' found for '{'.");
}


int Parser::findRParenWithoutLParen(size_t start) const {
    size_t i = start;
    int depth = 0;

    while (i < tokens.size()) {
        if (tokens[i].type == LexerTokenType::L_PAREN) {
            depth++;
        } else if (tokens[i].type == LexerTokenType::R_PAREN) {
            if (depth == 0) {
                return static_cast<int>(i);
            }
            depth--;
        }
        i++;
    }
    return i;
}


int Parser::findEndOfDoWhile(size_t start) const {
    if (tokens[start].type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' after 'do' keyword.");
    }

    size_t doEnd = findMatchingBrace(start) + 1;

    if (tokens[doEnd].type != LexerTokenType::WHILE) {
        throw std::runtime_error("Expected 'while' after 'do' block.");
    }

    const size_t end = findEndOfParenBlock(doEnd + 1);

    return end;
}

int Parser::findEndOfForLoop(size_t start) const {
    size_t headerEnd = findEndOfParenBlock(start);
    size_t bodyEnd = findMatchingBrace(headerEnd);

    return bodyEnd + 1;
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

int Parser::findEndOfIfElse(size_t start) const {
    size_t i = start;
    if (tokens[i].type != LexerTokenType::IF) return start;
    i++;
    i = findEndOfParenBlock(i);
    i = findMatchingBrace(i);
    int next = i + 1;
    while (next < static_cast<int>(tokens.size()) && tokens[next].type == LexerTokenType::ELSE) {
        next++;
        if (next < static_cast<int>(tokens.size()) && tokens[next].type == LexerTokenType::L_BRACE) {
            next = findMatchingBrace(next);
        }
        else if (next < static_cast<int>(tokens.size()) && tokens[next].type == LexerTokenType::IF) {
            next++;
            next = findEndOfParenBlock(next);
            next = findMatchingBrace(next) + 1;
        }
        else break;
    }
    return next;
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

std::unique_ptr<StatementNode> Parser::delegateToStatement(size_t i) {
    const auto index = i == -1 ? tokens.size() - 1 : i;
    const auto sliced = slice(index);
    auto stateParser = ParserStatement(sliced);
    auto statement = stateParser.parseStatement();
    this->current += stateParser.current;
    return std::move(statement);
}

std::unique_ptr<StatementNode> Parser::delegateToDeclaration(size_t i) {
    const auto index = i == -1 ? tokens.size() - 1 : i;
    const auto sliced = slice(index);
    auto stateParser = ParserDeclaration(sliced);
    auto statement = stateParser.parseDeclaration();
    this->current += stateParser.current + 1;
    return std::move(statement);
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
    return tokens.size() <= current || tokens.at(current).type == LexerTokenType::EOS;
}

bool Parser::isDeclaration(const LexerToken &token) {
    return token.type == LexerTokenType::VAR ||
           token.type == LexerTokenType::VAL ||
           token.type== LexerTokenType::FN;
}

bool Parser::isStatement(const LexerToken &token) {
    return token.type == LexerTokenType::DO ||
            token.type == LexerTokenType::PRINTLN ||
            token.type == LexerTokenType::RETURN ||
            token.type == LexerTokenType::IF ||
            token.type == LexerTokenType::WHILE ||
            token.type == LexerTokenType::FOR ||
            token.type == LexerTokenType::INSERT_  ||
            token.type == LexerTokenType::REMOVE_;
}


bool Parser::isNotExpression(const LexerToken &token) {
    return token.type == LexerTokenType::L_BRACE ||
           token.type == LexerTokenType::R_BRACE ||
           token.type == LexerTokenType::SEMICOLON ||
           token.type == LexerTokenType::ELSE && (!isDeclaration(token) && !isStatement(token));
}


std::unique_ptr<AST> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> ast;
    while (!isAtEnd()) {
        if (isDeclaration(peek())) {
            int end;
            try {
                if (peek().type == LexerTokenType::VAR || peek().type == LexerTokenType::VAL) end = findEndOfExpression(current) + 1;
                else end = findEndBraceDisconsideredFirst(current) + 1;
            }
            catch (...) {
                end = findEndOfExpression(current) + 1;
            }
            ast.push_back(delegateToDeclaration(end));
            current = end;
        }
        else if (isStatement(peek())) {
            int end;

            try {
                if (peek().type == LexerTokenType::PRINTLN ||
                    peek().type == LexerTokenType::INSERT_ ||
                    peek().type == LexerTokenType::REMOVE_ || peek().type == LexerTokenType::RETURN) end = findEndOfExpression(current);
                else if (peek().type == LexerTokenType::IF) end = findEndOfIfElse(current);
                else if (peek().type == LexerTokenType::DO) end = findEndOfDoWhile(current + 1);
                else if (peek().type == LexerTokenType::FOR) end = findEndOfForLoop(current + 1);
                else end = findEndBraceDisconsideredFirst(current) + 1;
            }
            catch (std::exception &e) {
                end = findEndOfExpression(current) + 1;
            }

            ast.push_back(delegateToStatement(end));
            current = end;
        }
        else if (!isNotExpression(peek())) {
            ast.push_back(delegateToExpression(findEndOfExpression(current) + 1));
            current += 1;
        }
        else advance();
    }
    return std::make_unique<AST>(std::move(ast));
}
