//
// Created by home on 19/04/25.
//

#include "ir_gen_expression.h"

#include <iostream>
#include "ir_instructions.h"



std::vector<std::string> IRGenExpression::binExpression(const BinaryExprNode* expression) {


    std::vector<std::string> bytecode;

    const auto bin = expression;

    auto leftBytecode = generateExpression(bin->left.get());

    auto rightBytecode = generateExpression(bin->right.get());

    bytecode.insert(bytecode.end(), leftBytecode.begin(), leftBytecode.end());
    bytecode.insert(bytecode.end(), rightBytecode.begin(), rightBytecode.end());
    bytecode.push_back(IRMapper::getOperator(bin->op) + " : i32");

    return bytecode;

}


std::string IRGenExpression::number(const NumberNode* number) {
    return IRMapper::getInstruction(IRInstruction::CONST) + " " + number->value + " : i32";
}

std::string IRGenExpression::identifier(const IdentifierExprNode* identifier) {
    return IRMapper::getInstruction(IRInstruction::LOAD) + " " + identifier->name + " : i32";
}



std::vector<std::string> IRGenExpression::generateExpression(const ExpressionNode* expression) {
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