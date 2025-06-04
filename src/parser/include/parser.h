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
    std::vector<std::string> permittedSizesInteger = {"8", "16", "32", "64"};
    std::vector<std::string> permittedSizesFloat = {"32", "64"};
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
    [[nodiscard]] int findEndBraceDisconsideredFirst(size_t start) const;
    [[nodiscard]] int findEndParenDisconsideredFirst(size_t start) const;
    [[nodiscard]] int findEndOfExpression(size_t start) const;
    [[nodiscard]] int findEndOfParenBlock(size_t start) const;
    [[ nodiscard ]] int findEndOfIfElse(size_t start) const;
    [[ nodiscard ]] int findEndOfDoWhile(size_t start) const;
    [[ nodiscard ]] int findEndOfForLoop(size_t start) const;
    [[ nodiscard ]] int findRParenWithoutLParen(size_t start) const;
    std::vector<LexerToken> tokensByCurrentBlock(int& blockEnd_);


private:
    bool isStatement(const LexerToken& token);
    bool isDeclaration(const LexerToken& token);
    bool isNotExpression(const LexerToken& token);
};

#endif //PARSER_H
