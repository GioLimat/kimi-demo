//
// Created by home on 19/04/25.
//

#include "bygen_expression.h"

#include <iostream>
#include "instructions.h"

std::string BygenExpression::getOpCode(const std::string& op, const std::string& type = "INT") {
    if (type == "INT") {
        if (op == "+") return opcodeToString.at(Opcode::ADD_I32);
        if (op == "-") return opcodeToString.at(Opcode::SUB_I32);
        if (op == "*") return opcodeToString.at(Opcode::MUL_I32);
        if (op == "/") return opcodeToString.at(Opcode::DIV_I32);
    } else if (type == "FLOAT") {
        if (op == "+") return "ADD_F32";
        if (op == "-") return "SUB_F32";
        if (op == "*") return "MUL_F32";
        if (op == "/") return "DIV_F32";
    }
    return "UNDEFINED";
}


std::vector<std::string> BygenExpression::binExpression(const BinaryExprNode* expression) {


    std::vector<std::string> bytecode;

    const auto bin = expression;

    auto leftBytecode = generateExpression(bin->left.get());

    auto rightBytecode = generateExpression(bin->right.get());

    bytecode.insert(bytecode.end(), leftBytecode.begin(), leftBytecode.end());
    bytecode.insert(bytecode.end(), rightBytecode.begin(), rightBytecode.end());
    bytecode.push_back(getOpCode(bin->op));

    return bytecode;

}


std::string BygenExpression::number(const NumberNode* number) {
    return opcodeToString.at(Opcode::CONST_I32) + " " + number->value;
}

std::string BygenExpression::identifier(const IdentifierExprNode* identifier) {
    return opcodeToString.at(Opcode::LOAD) + " " + identifier->name;
}



std::vector<std::string> BygenExpression::generateExpression(const ExpressionNode* expression) {
    std::vector<std::string> bytecode;
    if (auto binExpr = dynamic_cast<const BinaryExprNode*>(expression)) {
        auto bin = binExpression(binExpr);
        if (bin.data()) bytecode.insert(bytecode.end(), bin.begin(), bin.end());
    }
    else if (const auto num = dynamic_cast<const NumberNode*>(expression)) {
        const auto numBytecode = number(num);
        bytecode.push_back(numBytecode);
    }
    else if (const auto ident = dynamic_cast<const IdentifierExprNode*>(expression)) {
        const auto identifierBytecode = identifier(ident);
        bytecode.push_back(identifierBytecode);
    }


    return bytecode;
}