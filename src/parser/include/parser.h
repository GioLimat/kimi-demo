//
// Created by home on 11/04/25.
//

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>

#include "lexer_tokens.h"
#include "ast.h"
#include <optional>
#include <ranges>

class ParserException;

class Parser {
public:
    explicit Parser(const std::vector<LexerToken>& tokens);
    std::unique_ptr<AST> parse();
    size_t current = 0;
protected:
    std::vector<LexerToken> tokens;


    [[nodiscard]] LexerToken peek() const;
    [[nodiscard]] std::optional<LexerToken> previous() const;
    LexerToken advance();
    std::vector<LexerToken> slice(size_t i);
    std::unique_ptr<ExpressionNode> delegateToExpression(size_t i = INT32_MAX);
    std::unique_ptr<StatementNode> delegateToStatement(size_t i = INT32_MAX);
    std::unique_ptr<StatementNode> delegateToDeclaration(size_t i = INT32_MAX);
    [[nodiscard]] bool isAtEnd() const;
    [[nodiscard]] int findIndex(size_t start, LexerTokenType token) const;
    [[nodiscard]] int findMatchingBrace(int start) const;
    [[nodiscard]] int findEndOfExpression(size_t start) const;
    [[nodiscard]] int findEndOfParenBlock(size_t start) const;
    std::vector<LexerToken> tokensByCurrentBlock(int& blockEnd_);

private:
    bool isStatement(const LexerToken& token);
    bool isDeclaration(const LexerToken& token);
};

#endif //PARSER_H
