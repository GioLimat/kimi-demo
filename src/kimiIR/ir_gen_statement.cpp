//
// Created by home on 02/05/25.
//

#include <iostream>

#include "ir_instructions.h"
#include "ir_gen.h"


void IRGen::visitBlockStatement(BlockStatementNode *node) {
    for (const auto& n : node->statements) {
        n->accept(*this);
    }
}


void IRGen::visitIfStatement(IfStatementNode *node) {
    node->condition->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF_FALSE) + " L" + std::to_string(currentLabel));

    for (auto &n : node->thenBranch->statements) {
        n->accept(*this);
    }
    if (node->elseBranch) {
        currentLabel++;
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::JMP) + " L" + std::to_string(currentLabel));
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel - 1));
        node->elseBranch->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel));
}


void IRGen::visitWhileStatement(WhileStatementNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::WHILE));
    node->condition->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_CONDITION));
    for (const auto& n : node->body->statements) {
        n->accept(*this);
    }

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
}

void IRGen::visitDoWhileStatement(DoWhileStatementNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::DO));
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    for (const auto& n : node->whileStatement->body->statements) {
        n->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    node->whileStatement->condition->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_CONDITION));
}

void IRGen::visitPrintln(PrintlnStatementNode *node) {
    node->expression->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::PRINT) + " : " + node->type);
}


void IRGen::visitReturnStatement(ReturnStatementNode *node) {
    if (node->returnValue) {
        node->returnValue->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::RET) + " : " + node->returnType);
}




void IRGen::visitExpressionStatement(ExpressionStatementNode *node) {
    if (node->expression) node->expression->accept(*this);
}
