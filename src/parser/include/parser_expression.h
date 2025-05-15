//
// Created by home on 11/04/25.
//

#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include "parser.h"


class ParserExpression : public Parser {
public:
    std::unique_ptr<ExpressionNode> parseExpression();
    explicit ParserExpression(const std::vector<LexerToken>& tokens): Parser(tokens) {};
protected:
    [[nodiscard]] int precedence(LexerTokenType type) ;
    std::unique_ptr<ExpressionNode> parseBinaryOperation(int minPrecedence);
    std::unique_ptr<ExpressionNode> parsePrimary();
    std::unique_ptr<ExpressionNode> parseUnary();
    std::unique_ptr<ExpressionNode> parseCallIdentifier();
    std::unique_ptr<ExpressionNode> parseAssignment();
    std::unique_ptr<ExpressionNode> parsePostFix();
    std::unique_ptr<ExpressionNode> parseComposeAssignment();
};

#endif //PARSER_EXPRESSION_H
