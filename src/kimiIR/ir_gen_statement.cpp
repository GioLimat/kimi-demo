//
// Created by home on 02/05/25.
//

#include "ir_instructions.h"
#include "ir_gen.h"


void IRGen::visitIfStatement(IfStatementNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF));
    node->condition->accept(*this);

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_CONDITION));
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    for (const auto& n : node->thenBranch->statements) {
        n->accept(*this);
    }
    if (node->elseBranch) {
        node->elseBranch->accept(*this);
    }

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
}


void IRGen::visitWhileStatement(WhileStatementNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::WHILE));
    node->condition->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_CONDITION));
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
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
