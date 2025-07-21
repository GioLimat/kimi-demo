//
// Created by home on 14/04/25.
//

#include "parse_declaration.h"

#include <algorithm>
#include <ranges>
#include "sizes.h"



void ParserDeclaration::extractType(std::string &type, std::string assumedType, std::vector<std::string> sizes) {
    advance();
    type = assumedType;
    if (peek().type == LexerTokenType::L_BRACKET) {
        advance();
        if (peek().type != LexerTokenType::NUMBER_INT) throw std::runtime_error("Expected number int");
        if (sizes.end() == std::find(sizes.begin(), sizes.end(), peek().value)) {
            throw std::runtime_error("Expected " + assumedType + "  size");
        }
        type = assumedType[0] + peek().value;
        advance();
        if (advance().type != LexerTokenType::R_BRACKET) throw std::runtime_error("Expected ']'");
    }
}



std::string ParserDeclaration::getType() {
    std::string type;

    if (peek().type == LexerTokenType::ARRAY) {
        advance();
        if (advance().type != LexerTokenType::L_BRACKET)
            throw std::runtime_error("Expected '[' after ARRAY");
        std::string inner = getType();

        if (advance().type != LexerTokenType::R_BRACKET)
            throw std::runtime_error("Expected ']' after array element type");

        type = "array<" + inner + ">";
    }

    if (peek().type == LexerTokenType::INT) extractType(type, "i32", intSizes);
    else if (peek().type == LexerTokenType::FLOAT) extractType(type, "f32", floatSizes);
    else if (peek().type == LexerTokenType::UINT) extractType(type, "u32", intSizes);
    else if (peek().type == LexerTokenType::CHAR) {
        advance();
        type = "char";
    }
    else if (peek().type == LexerTokenType::BOOLEAN) {
        advance();
        type = "bool";
    }
    else if (peek().type == LexerTokenType::STR) {
        advance();
        type = "str";
    }
    return type;
}


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
    std::string type;
    const auto name = advance();

    if (name.type != LexerTokenType::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after var/val");
    }

    if (peek().type == LexerTokenType::COLON) {
        advance();
        std::string tempType = getType();
        if (!tempType.empty()) {
            type = tempType;
        }
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
    VarDeclarationNode var(name.value, std::move(initializer), isConst);
    var.declaredType = type;

    return std::make_unique<VarDeclarationNode>(std::move(var));
}

std::unique_ptr<StatementNode> ParserDeclaration::parseFunctionDeclaration() {
    const auto name = advance();

    if (name.type != LexerTokenType::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after function declaration");
    }

    if (const auto lBrace = advance(); lBrace.type != LexerTokenType::L_PAREN) {
        throw std::runtime_error("Expected '(' after function name");
    }



    std::vector<FunctionDeclarationNode::Param> parameters;

    if (peek().type != LexerTokenType::R_PAREN) {
        size_t i = 0;
        while (true) {
            if (peek().type != LexerTokenType::IDENTIFIER) {
                throw std::runtime_error("Expected parameter name");
            }
            parameters.push_back({});
            parameters[i].name = advance().value;


            if (advance().type != LexerTokenType::COLON) {
               throw std::runtime_error("Expected ':' after parameter");
            }


            if (peek().type != LexerTokenType::INT &&
                peek().type != LexerTokenType::FLOAT
                && peek().type != LexerTokenType::CHAR &&
                peek().type != LexerTokenType::BOOLEAN &&
                peek().type != LexerTokenType::STR &&
                peek().type != LexerTokenType::UINT &&
                peek().type != LexerTokenType::ARRAY
            ) {
                throw std::runtime_error("Expected parameter type");
            }

            parameters[i].type = getType();

            if (peek().type == LexerTokenType::COMMA) {
                advance();
                i++;
            } else {
                break;
            }
        }
    }

    if (advance().type != LexerTokenType::R_PAREN) {
        throw std::runtime_error("Expected ')' after function parameters");
    }
    std::string type = "infer";
    if (peek().type == LexerTokenType::COLON) {
        advance();
        type = getType();
    }

    if (peek().type != LexerTokenType::L_BRACE) {
        throw std::runtime_error("Expected '{' after function body");
    }


    int blockEnd = -1;
    auto sliced = tokensByCurrentBlock(blockEnd);

    auto body = std::vector<std::unique_ptr<ASTNode>>();
    if (!sliced.empty()) {
        auto parser = Parser(sliced);
        body =  std::move(parser.parse()->children);
    }
    current = blockEnd + 1;


    if (!body.empty()) {
        auto lastPtr = std::move(body.back());
        body.pop_back();

        if (!dynamic_cast<ReturnStatementNode*>(lastPtr.get())) {
            if (auto expr = dynamic_cast<ExpressionNode*>(lastPtr.get())) {
                std::unique_ptr<ExpressionNode> exprPtr(
                    dynamic_cast<ExpressionNode*>(lastPtr.release())
                );
                body.push_back(
                    std::make_unique<ReturnStatementNode>(std::move(exprPtr))
                );
            }
            else if (auto state = dynamic_cast<StatementNode*>(lastPtr.get())) {
                body.push_back(std::move(lastPtr));
                body.push_back(std::make_unique<ReturnStatementNode>(nullptr));
            }
        }
        else {
            body.push_back(std::move(lastPtr));
        }
    }
    else {
        body.push_back(std::make_unique<ReturnStatementNode>(nullptr));
    }


    advance();

    auto fn = FunctionDeclarationNode(name.value, std::move(parameters), std::make_unique<BlockStatementNode>(std::move(body)));
    fn.returnType = type;

    return std::make_unique<FunctionDeclarationNode>(std::move(fn));
}