//
// Created by home on 14/04/25.
//
#include <ranges>
#include "parser_statement.h"

#include <parser_expression.h>





std::unique_ptr<StatementNode> ParserStatement::parseStatement() {
    const auto token = peek();

    if (token.type == LexerTokenType::RETURN) return parseReturnStatement();
    if (token.type == LexerTokenType::IF) return parseIfStatement();
    if (token.type == LexerTokenType::WHILE) return parseWhileStatement();
    if (token.type == LexerTokenType::DO) return parseDoWhileStatement();
    if (token.type == LexerTokenType::FOR) return parseForStatement();
    if (token.type == LexerTokenType::PRINTLN) return parsePrintln();
    if (token.type == LexerTokenType::INSERT_ || token.type == LexerTokenType::REMOVE_) return parseInsertRemoveStatement();

    const auto end = findEndOfExpression(current);
    auto expr = delegateToExpression(end);

    return std::make_unique<ExpressionStatementNode>(std::move(expr));
}


std::unique_ptr<StatementNode> ParserStatement::parseReturnStatement() {
    advance();

    if (peek().type == LexerTokenType::SEMICOLON || peek().type == LexerTokenType::R_BRACE) {
        return std::make_unique<ReturnStatementNode>(nullptr);
    }


    const int exprEnd = findEndOfExpression(current);
    if (exprEnd == -1) {
        throw std::runtime_error("Expected ';' or '}' after return expression");
    }
    auto expr = delegateToExpression(exprEnd);
    current = exprEnd;
    advance();

    return std::make_unique<ReturnStatementNode>(std::move(expr));
}



std::unique_ptr<StatementNode> ParserStatement::parseIfStatement() {
    advance();

    const int end = findEndOfParenBlock(current);
    auto condition = delegateToExpression(end);
    current = end;



    if (peek().type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' after if condition");
    }

    int blockEnd = -1;
    auto sliced = tokensByCurrentBlock(blockEnd);
    auto finalEnd = findMatchingBrace(current) + 1;
    auto body = std::vector<std::unique_ptr<ASTNode>>();

    if (!sliced.empty()) {
        auto parser = Parser(sliced);
        body = std::move(parser.parse()->children);
    }
    current = finalEnd;
    std::unique_ptr<StatementNode> elseBranch = nullptr;
    if (peek().type == LexerTokenType::ELSE) {
        advance();
        if (peek().type == LexerTokenType::IF) {
            elseBranch = parseIfStatement();
        }
        else if (peek().type == LexerTokenType::L_BRACE) {
            int blockEndElse = -1;
            auto slicedElse = tokensByCurrentBlock(blockEndElse);
            int finalEndElse = findMatchingBrace(current) + 1;
            std::vector<std::unique_ptr<ASTNode>> bodyElse;
            if (!slicedElse.empty()) {
                auto parserElse = Parser(slicedElse);
                bodyElse = std::move(parserElse.parse()->children);
            }

            current = finalEndElse;
            elseBranch = std::make_unique<BlockStatementNode>(std::move(bodyElse));
        }
    }
    return std::make_unique<IfStatementNode>(
        std::move(condition),
        std::make_unique<BlockStatementNode>(std::move(body)),
        std::move(elseBranch)
    );
}


std::unique_ptr<StatementNode> ParserStatement::parseWhileStatement() {
    advance();

    const int end = findEndOfParenBlock(current);
    auto condition = delegateToExpression(end);
    current = end;

    if (peek().type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' after while condition");
    }

    int blockEnd;
    auto sliced = tokensByCurrentBlock(blockEnd);
    auto body = std::make_unique<BlockStatementNode>(std::move(Parser(sliced).parse()->children));
    current = blockEnd;

    return std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));
}

std::unique_ptr<StatementNode> ParserStatement::parseDoWhileStatement() {
    advance();
    int blockEnd;
    auto sliced = tokensByCurrentBlock(blockEnd);



    auto body = std::make_unique<BlockStatementNode>(std::move(Parser(sliced).parse()->children));
    current = blockEnd + 1;


    if (peek().type != LexerTokenType::WHILE) {
        throw std::runtime_error("Expected 'while' after do-while block");
    }
    advance();

    const int end = findEndOfParenBlock(current);
    auto condition = delegateToExpression(end);
    current = end;

    auto whileStatement = std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));

    return std::make_unique<DoWhileStatementNode>(std::move(whileStatement));
}



std::vector<std::unique_ptr<ASTNode>> ParserStatement::parseBlock(const int end) {
    std::vector<std::unique_ptr<ASTNode>> body;
    advance();



    while (current < end) {
        if (peek().type == LexerTokenType::R_BRACE || peek().type == LexerTokenType::SEMICOLON) {
            advance();
        }
        else
        body.push_back(parseStatement());
    }

    if (peek().type == LexerTokenType::SEMICOLON) advance();


    if (advance().type != LexerTokenType::R_BRACE) {
        throw std::runtime_error("Expected '}' at end of block");
    }

    return body;
}


std::unique_ptr<StatementNode> ParserStatement::parsePrintln() {
    advance();

    const int end = findEndOfParenBlock(current);
    auto expr = delegateToExpression(end);
    current = end;

    return std::make_unique<PrintlnStatementNode>(std::move(expr));
}



std::unique_ptr<StatementNode> ParserStatement::parseForStatement() {
    advance();

    if (advance().type != LexerTokenType::L_PAREN) {
        throw std::runtime_error("Expected '(' after 'for'");
    }

    std::unique_ptr<ASTNode> initializer = nullptr;

    if (peek().type == LexerTokenType::VAR || peek().type == LexerTokenType::VAL) {
        int initEnd = findEndOfExpression(current);
        initializer = delegateToDeclaration(initEnd);
        current = initEnd + 1;

    } else {
        uint64_t initEnd = findEndOfExpression(current);
        initializer = delegateToExpression(initEnd);
        current = initEnd + 1;
    }


    std::unique_ptr<ExpressionNode> condition = nullptr;

    uint64_t conditionEnd = findEndOfExpression(current);
    condition = delegateToExpression(conditionEnd);
    current = conditionEnd + 1;

    std::unique_ptr<ExpressionNode> posBody = nullptr;


    uint64_t posEnd = findRParenWithoutLParen(current);
    posBody = delegateToExpression(posEnd);
    current = posEnd + 1;

    int blockEnd = -1;
    auto sliced = tokensByCurrentBlock(blockEnd);
    auto finalEnd = findMatchingBrace(current) + 1;
    auto body = std::vector<std::unique_ptr<ASTNode>>();

    if (!sliced.empty()) {
        auto parser = Parser(sliced);
        body = std::move(parser.parse()->children);
    }
    current = finalEnd;


    return std::make_unique<ForStatementNode>(
        std::move(initializer),
        std::move(condition),
        std::move(posBody),
        std::make_unique<BlockStatementNode>(std::move(body))
    );
}


std::unique_ptr<StatementNode> ParserStatement::parseInsertRemoveStatement() {
    return nullptr;
}
