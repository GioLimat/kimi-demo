//
// Created by home on 19/04/25.
//

#include "bygen_expression.h"

#include <iostream>


std::string BygenExpression::getOpCode(const std::string& op, const std::string& type = "INT") {
    if (type == "INT") {
        if (op == "+") return "ADD_I32";
        if (op == "-") return "SUB_I32";
        if (op == "*") return "MUL_I32";
        if (op == "/") return "DIV_I32";
    } else if (type == "FLOAT") {
        if (op == "+") return "ADD_F32";
        if (op == "-") return "SUB_F32";
        if (op == "*") return "MUL_F32";
        if (op == "/") return "DIV_F32";
    }
    return "UNDEFINED";
}


std::vector<std::string> BygenExpression::binExpression(const std::unique_ptr<ExpressionNode> &expression) {
    if (auto bin = dynamic_cast<BinaryExprNode*>(expression.get())) {

        std::vector<std::string> bytecode;

        auto leftBytecode = generateExpression(bin->left);

        auto rightBytecode = generateExpression(bin->right);

        bytecode.insert(bytecode.end(), leftBytecode.begin(), leftBytecode.end());
        bytecode.insert(bytecode.end(), rightBytecode.begin(), rightBytecode.end());
        bytecode.push_back(getOpCode(bin->op));

        return bytecode;
    }

    return {};
}


std::string BygenExpression::number(const NumberNode* number) {
    return "CONST_I32 " + number->value;
}

std::string BygenExpression::identifier(const IdentifierExprNode* identifier) {
    return "LOAD " + identifier->name;
}



std::vector<std::string> BygenExpression::generateExpression(const std::unique_ptr<ExpressionNode>& expression) {
    std::vector<std::string> bytecode;
    if (auto binExpr = dynamic_cast<BinaryExprNode*>(expression.get())) {
        auto bin = binExpression(expression);
        if (bin.data()) bytecode.insert(bytecode.end(), bin.begin(), bin.end());
    }
    else if (const auto num = dynamic_cast<NumberNode*>(expression.get())) {
        const auto numBytecode = number(num);
        bytecode.push_back(numBytecode);
    }
    else if (const auto ident = dynamic_cast<IdentifierExprNode*>(expression.get())) {
        const auto identifierBytecode = identifier(ident);
        bytecode.push_back(identifierBytecode);
    }


    return bytecode;
}