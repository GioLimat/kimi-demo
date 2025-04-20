//
// Created by home on 14/04/25.
//

#include "parse_declaration.h"
#include <ranges>


std::unique_ptr<StatementNode> ParserDeclaration::parseDeclaration() {
    const auto& token = peek();

    if (token.type == LexerTokenType::VAR || token.type == LexerTokenType::VAL) return parseVarDeclaration();
    if (token.type == LexerTokenType::FN) {
        advance();
        return parseFunctionDeclaration();
    }

    throw std::runtime_error("Expceted var, val or fn");

}

std::unique_ptr<StatementNode> ParserDeclaration::parseVarDeclaration() {
    bool isConst = advance().type == LexerTokenType::VAL;

    const auto name = advance();

    if (name.type != LexerTokenType::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after var/val");
    }

    const auto nextToken = peek();
    std::unique_ptr<ExpressionNode> initializer = nullptr;

    if (nextToken.type == LexerTokenType::EQUALS) {
        advance();
        const int exprEnd = findEndOfExpression(current);
        if (exprEnd == -1) {
            throw std::runtime_error("var/val after expected ';' expression");
        }

        initializer = delegateToExpression(exprEnd);
    }

    return std::make_unique<VarDeclarationNode>(name.value, std::move(initializer), isConst);
}

std::unique_ptr<StatementNode> ParserDeclaration::parseFunctionDeclaration() {
    const auto name = advance();

    if (name.type != LexerTokenType::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after function declaration");
    }

    if (const auto lBrace = advance(); lBrace.type != LexerTokenType::L_PAREN) {
        throw std::runtime_error("Expected '(' after function name");
    }



    std::vector<std::string> parameters;

    if (peek().type != LexerTokenType::R_PAREN) {
        while (true) {
            if (peek().type != LexerTokenType::IDENTIFIER) {
                throw std::runtime_error("Expected parameter name");
            }

            parameters.push_back(advance().value);


            if (peek().type == LexerTokenType::COMMA) {
                advance();
            } else {
                break;
            }
        }
    }

    if (advance().type != LexerTokenType::R_PAREN) {
        throw std::runtime_error("Expected ')' after function parameters");
    }

    if (peek().type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' after function body");
    }


    int blockEnd = -1;
    auto sliced = tokensByCurrentBlock(blockEnd);

    auto stateParser = ParserStatement(sliced);
    std::vector<std::unique_ptr<StatementNode>> body = stateParser.parseBlock(blockEnd);

    current = blockEnd + 1;

    if (!body.empty()) {
        if (auto* last = body.back().get(); !dynamic_cast<ReturnStatementNode*>(last)) {
            if (auto* exprStmt = dynamic_cast<ExpressionStatementNode*>(last)) {
                body.back() = std::make_unique<ReturnStatementNode>(std::move(exprStmt->expression));
            }
        }
    }

    advance();

    return std::make_unique<FunctionDeclarationNode>(name.value, std::move(parameters), std::move(body));
}