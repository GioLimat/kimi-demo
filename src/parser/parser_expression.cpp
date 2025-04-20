//
// Created by home on 11/04/25.
//

#include "lexer_tokens.h"
#include "parser_expression.h"


int ParserExpression::precedence(const LexerTokenType type) {
    switch (type) {
        case LexerTokenType::MULTIPLY:
        case LexerTokenType::DIVIDE:
        case LexerTokenType::MODULUS:
            return 4;

        case LexerTokenType::PLUS:
        case LexerTokenType::MINUS:
            return 3;

        case LexerTokenType::EQUAL_EQUAL:
        case LexerTokenType::BANG_EQUAL:
        case LexerTokenType::LESS:
        case LexerTokenType::GREATER:
        case LexerTokenType::LESS_EQUAL:
        case LexerTokenType::GREATER_EQUAL:
            return 2;

        case LexerTokenType::AND_AND:
            return 1;

        case LexerTokenType::OR_OR:
            return 0;

        default:
            return -1;
    }
}


std::unique_ptr<ExpressionNode> ParserExpression::parseExpression() {
    if (isAtEnd()) return nullptr;
    if (peek().type == LexerTokenType::IDENTIFIER && tokens.at(current + 1).type == LexerTokenType::EQUALS) {
        auto asg = parseAssignment();
        if (asg != nullptr)
            return asg;
    }
    return parseBinaryOperation(1);
}


std::unique_ptr<ExpressionNode> ParserExpression::parseAssignment() {
    auto identifier = advance().value;
    advance();

    auto expr = parseBinaryOperation(1);

    return std::make_unique<AssignmentExprNode>(std::move(identifier), std::move(expr));
}


std::unique_ptr<ExpressionNode> ParserExpression::parseBinaryOperation(const int minPrecedence) {
    auto left = parseUnary();
    while (true) {
        const auto currentToken = peek();
        const int currentPrecedence = precedence(currentToken.type);

        if (currentPrecedence < minPrecedence || currentToken.type == LexerTokenType::EOS
            || currentToken.type == LexerTokenType::SEMICOLON) break;

        std::string op = advance().value;

        auto right = parseBinaryOperation(currentPrecedence + 1);

        left = std::make_unique<BinaryExprNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ExpressionNode> ParserExpression::parseCallIdentifier() {
    std::string functionName = advance().value;

    if (peek().type == LexerTokenType::L_PAREN) {
        advance();

        std::vector<std::unique_ptr<ExpressionNode>> arguments;

        if (peek().type != LexerTokenType::R_PAREN) {
            while (true) {
                auto arg = parseExpression();
                arguments.push_back(std::move(arg));

                if (peek().type == LexerTokenType::COMMA) {
                    advance();
                } else {
                    break;
                }
            }
        }

        if (peek().type != LexerTokenType::R_PAREN) {
            throw std::runtime_error("Expected ')' after arguments in function call");
        }

        advance();
        return std::make_unique<CallFunctionNode>(functionName, std::move(arguments));
    }

    return std::make_unique<IdentifierExprNode>(functionName);
}


std::unique_ptr<ExpressionNode> ParserExpression::parseUnary() {
    if (auto token = peek(); token.type == LexerTokenType::MINUS
        ||
        token.type == LexerTokenType::BANG
        ||
        token.type == LexerTokenType::PLUS
        ||
        token.type == LexerTokenType::MINUS_MINUS
        ||
        token.type == LexerTokenType::PLUS_PLUS) {
            std::string op = advance().value;
            auto operand = parseUnary();
            return std::make_unique<UnaryExprNode>(op, std::move(operand));
        }

        return parsePrimary();
}


std::unique_ptr<ExpressionNode> ParserExpression::parsePrimary() {
    auto token = peek();

    if (token.type == LexerTokenType::INT || token.type == LexerTokenType::FLOAT) {
        std::string value = advance().value;
        return std::make_unique<NumberNode>(value);
    }

    if (token.type == LexerTokenType::TRUE || token.type == LexerTokenType::FALSE) {
        bool value = advance().type == LexerTokenType::TRUE;
        return std::make_unique<BooleanNode>(value);
    }

    if (token.type == LexerTokenType::IDENTIFIER) {
        return parseCallIdentifier();
    }

    if (token.type == LexerTokenType::L_PAREN) {
        advance();
        auto expr = parseBinaryOperation(1);
        if (peek().type != LexerTokenType::R_PAREN) {
            throw std::runtime_error("Expected ')' after expression");
        }
        advance();
        return expr;
    }

    if (token.type == LexerTokenType::EOS) {
        return nullptr;
    }

    throw std::runtime_error("Unexpected token: " + token.value);

}



