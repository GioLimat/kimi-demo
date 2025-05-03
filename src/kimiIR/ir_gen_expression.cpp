//
// Created by home on 19/04/25.
//

#include "ir_gen.h"
#include "ir_instructions.h"


void IRGen::visitBinaryExpr(BinaryExprNode *expression) {
    auto bin = expression;

    bin->left->accept(*this);
    bin->right->accept(*this);
    bytecode.push_back(IRMapper::getOperator(bin->op) + " : " + bin->type);
}

void IRGen::visitNumber(NumberNode *number) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CONST) + " " + number->value + " : " + number->type);
}



void IRGen::visitIdentifier(IdentifierExprNode *identifier) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LOAD) + " " + identifier->name + " : " + identifier->type);
}


void IRGen::visitCallFunction(CallFunctionNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CALL) + " " + node->name);
    for (const auto &arg : node->arguments) {
        arg->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CALL_END) + " " + node->name);
}


void IRGen::visitGenericExpressionNode(GenericExpressionNode *node) {
    node->node->accept(*this);
}
