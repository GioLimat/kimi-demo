//
// Created by home on 11/04/25.
//

#include "lexer_tokens.h"
#include "parser_expression.h"

#include <codecvt>
#include <limits>
#include <locale>


int ParserExpression::precedence(const LexerTokenType type) {
    switch (type) {
        case LexerTokenType::MULTIPLY:
        case LexerTokenType::DIVIDE:
        case LexerTokenType::MODULUS:
            return 8;

        case LexerTokenType::PLUS:
        case LexerTokenType::MINUS:
        case LexerTokenType::PLUS_EQUAL:
            return 7;

        case LexerTokenType::SHIFT_LEFT:
        case LexerTokenType::SHIFT_RIGHT:
            return 6;

        case LexerTokenType::BIT_AND:
            return 5;

        case LexerTokenType::XOR:
            return 4;

        case LexerTokenType::BIT_OR:
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

static std::string swapTypes(const std::string& type) {
    auto pos = type.find('[');
    if (pos == std::string::npos) {
        if (type == "Int")   return "i32";
        if (type == "Float") return "f32";
        if (type == "Bool")  return "bool";
        if (type == "Char") return "char";
        if (type == "Str") return "str";
        if (type == "Uint") return "u32";
        return type;
    } else {
        auto base = type.substr(0, pos);
        auto num  = type.substr(pos + 1, type.find(']') - pos - 1);
        if (base == "Int")   return "i" + num;
        if (base == "Float") return "f" + num;
        if (base == "Uint") return "u" + num;
        return type;
    }
}

std::unique_ptr<ExpressionNode> ParserExpression::parseCastOrUnary() {
    auto left = parseUnary();

    if (peek().type == LexerTokenType::AS) {
        advance();

        std::string target = advance().value;

        if (peek().type == LexerTokenType::L_BRACKET) {
            advance();
            std::string num = advance().value;
            advance();
            target += "[" + num + "]";
        }

        auto castNode = std::make_unique<CastingExpressionNode>(std::move(left));
        castNode->targetType  = std::move(swapTypes(target));
        return std::move(castNode);
    }

    return left;
}




std::unique_ptr<ExpressionNode> ParserExpression::parseLeftHandSide() {
    auto lhs = parseCallIdentifier();
    while (peek().type == LexerTokenType::L_BRACKET)
    {
        if (peek().type == LexerTokenType::L_BRACKET) {
            advance();
            auto idx = parseExpression();
            if (peek().type != LexerTokenType::R_BRACKET)
                throw std::runtime_error("Expected ]");
            advance();
            lhs = std::make_unique<IndexAccessExpr>(std::move(lhs), std::move(idx), true);
        }
    }
    return lhs;
}

std::unique_ptr<ExpressionNode> ParserExpression::parseExpression() {
    if (isAtEnd()) return nullptr;
    if (peek().type == LexerTokenType::IDENTIFIER) {
        size_t save = current;
        auto lhs = parseLeftHandSide();
        auto tok = peek().type;
        bool isEq      = tok == LexerTokenType::EQUALS;
        bool isCompose = tok == LexerTokenType::PLUS_EQUAL
                      || tok == LexerTokenType::MINUS_EQUAL
                      || tok == LexerTokenType::MULTIPLY_EQUAL
                      || tok == LexerTokenType::DIVIDE_EQUAL
                      || tok == LexerTokenType::MODULUS_EQUAL
                      || tok == LexerTokenType::SHIFT_LEFT_EQUAL
                      || tok == LexerTokenType::SHIFT_RIGHT_EQUAL
                      || tok == LexerTokenType::BIT_AND_EQUAL
                      || tok == LexerTokenType::BIT_OR_EQUAL
                      || tok == LexerTokenType::XOR_EQUAL;


        if (const auto* idx = dynamic_cast<IndexAccessExpr*>(lhs.get()); idx != nullptr && isEq) {
            current = save;
            return parseAssignmentIndexExpression();
        }
        if (isEq || isCompose) {
            current = save;
            if (isCompose) return parseComposeAssignment();
            return parseAssignment();
        }
        current = save;
    }
    if (peek().type == LexerTokenType::IDENTIFIER
        && (tokens.at(current + 1).type == LexerTokenType::PLUS_PLUS
         || tokens.at(current + 1).type == LexerTokenType::MINUS_MINUS))
    {
        return parsePostFix();
    }
    return parseBinaryOperation(0);
}

std::unique_ptr<ExpressionNode> ParserExpression::parseAssignmentIndexExpression() {
    auto name = peek().value;
    auto left = parseLeftHandSide();
    if (peek().type != LexerTokenType::EQUALS) {
        throw std::runtime_error("Expected '=' after index expression");
    }
    advance();
    auto value = parseExpression();

    return std::make_unique<AssignmentIndexExprNode>(
        move(name), move(value), move(left)
        );
}


std::unique_ptr<ExpressionNode> ParserExpression::parseBinaryOperation(const int minPrecedence) {
    auto left = parseCastOrUnary();
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



std::unique_ptr<ExpressionNode> ParserExpression::parsePostFix() {

    auto identifier = parseCallIdentifier();

    if (auto id = dynamic_cast<IdentifierExprNode*>(identifier.get()); id == nullptr) throw std::runtime_error("Expected identifier");
    return std::make_unique<PostFixExprNode>(advance().value, std::move(identifier));
}


std::unique_ptr<ExpressionNode> ParserExpression::parseAssignment() {
    auto target = parseLeftHandSide();
    advance();
    auto expr = parseExpression();
    std::string name;
    if (auto id = dynamic_cast<IdentifierExprNode*>(target.get())) {
        name = id->name;
    }
    return std::make_unique<AssignmentExprNode>(
        std::move(name),
        std::move(expr),
        std::move(target)
    );
}


std::unique_ptr<ExpressionNode> ParserExpression::parseComposeAssignment() {
    std::string varName = advance().value;
    std::string compOp = advance().value;

    auto rhs = parseBinaryOperation(0);

    char binOp = compOp[0];

    auto leftIdent = std::make_unique<IdentifierExprNode>(varName);
    auto addExpr = std::make_unique<BinaryExprNode>(
        std::string(1, binOp),
        std::move(leftIdent),
        std::move(rhs)
    );

    return std::make_unique<AssignmentExprNode>(varName, std::move(addExpr), nullptr);
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
        token.type == LexerTokenType::PLUS_PLUS
        ||
        token.type == LexerTokenType::BIT_NOT) {
            std::string op = advance().value;
            auto operand = parseUnary();
            return std::make_unique<UnaryExprNode>(op, std::move(operand));
        }
        return parsePrimary();
}

std::string inferIntegerType(const std::string& valueStr) {
    long long value = std::stoll(valueStr);

    if (value >= std::numeric_limits<int8_t>::min() && value <= std::numeric_limits<int8_t>::max())
        return "i8";
    if (value >= std::numeric_limits<int16_t>::min() && value <= std::numeric_limits<int16_t>::max())
        return "i16";
    if (value >= std::numeric_limits<int32_t>::min() && value <= std::numeric_limits<int32_t>::max())
        return "i32";

    return "i64";
}

std::string inferFloatType(const std::string& valueStr) {
    double value = std::stod(valueStr);
    float fvalue = static_cast<float>(value);

    double diff = std::abs(static_cast<double>(fvalue) - value);
    const double epsilon = 1e-6;

    if (diff < epsilon) {
        return "f32";
    }
    return "f64";
}

std::unique_ptr<ExpressionNode> ParserExpression::parsePrimary() {
    std::unique_ptr<ExpressionNode> node;
    auto token = peek();

    if (token.type == LexerTokenType::L_BRACKET) {
        advance();
        std::vector<std::unique_ptr<ExpressionNode>> elements;
        if (peek().type != LexerTokenType::R_BRACKET) {
            while (true) {
                auto elem = parseExpression();
                elements.push_back(std::move(elem));
                if (peek().type == LexerTokenType::COMMA) {
                    advance();
                    continue;
                }
                break;
            }
        }
        if (peek().type != LexerTokenType::R_BRACKET)
            throw std::runtime_error("Expected ']' after array literal");
        advance();
        node = std::make_unique<ArrayLiteralNode>(std::move(elements));
    }

    else if (token.type == LexerTokenType::NUMBER_INT || token.type == LexerTokenType::NUMBER_FLOAT) {
        std::string value;
        value = advance().value;
        std::string type;
        if (token.type == LexerTokenType::NUMBER_INT) {
            type = inferIntegerType(token.value);
        } else {
            type = inferFloatType(token.value);
        }
        node = std::make_unique<NumberNode>(value, type);
    }
    else if (token.type == LexerTokenType::CHAR_LITERAL) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        auto u32 = conv.from_bytes(advance().value);
        if (u32.empty()) throw std::runtime_error("Invalid UTF-8 in char literal");
        node = std::make_unique<CharLiteralExpr>(static_cast<uint32_t>(u32[0]));
    }
    else if (token.type == LexerTokenType::STR_LITERAL) {
        std::string lit = advance().value;
        node = std::make_unique<StringLiteralExpr>(std::move(lit));
    }
    else if (token.type == LexerTokenType::TRUE || token.type == LexerTokenType::FALSE) {
        bool v = advance().type == LexerTokenType::TRUE;
        node = std::make_unique<BooleanNode>(v);
    }
    else if (token.type == LexerTokenType::IDENTIFIER || token.type == LexerTokenType::INSERT_ || token.type == LexerTokenType::REMOVE_) {
        node = parseCallIdentifier();
    }
    else if (token.type == LexerTokenType::L_PAREN) {
        advance();
        auto inner = parseExpression();
        if (peek().type != LexerTokenType::R_PAREN) throw std::runtime_error("Expected ')' after expression");
        advance();
        node = std::move(inner);
    }
    else {
        return nullptr;
    }

    while (peek().type == LexerTokenType::L_BRACKET) {
        advance();
        auto idxExpr = parseExpression();
        if (peek().type != LexerTokenType::R_BRACKET)
            throw std::runtime_error("Expected ']' after index expression");
        advance();
        node = std::make_unique<IndexAccessExpr>(
            std::move(node),
            std::move(idxExpr),
            true
        );
    }

    return node;
}
