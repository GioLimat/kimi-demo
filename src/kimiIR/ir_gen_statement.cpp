//
// Created by home on 02/05/25.
//

#include <iostream>

#include "ir_instructions.h"
#include "ir_gen.h"
#include "type_analyzer.h"


void IRGen::visitBlockStatement(BlockStatementNode *node) {
    scopes.emplace();
    for (const auto& n : node->statements) {
        n->accept(*this);
    }
    scopes.pop();
}


void IRGen::visitIfStatement(IfStatementNode *node) {
    scopes.emplace();
    node->condition->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF_FALSE) + " L" + std::to_string(currentLabel) + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    for (auto &n : node->thenBranch->statements) {
        n->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    scopes.pop();
    if (node->elseBranch) {
        currentLabel++;
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::JMP) + " L" + std::to_string(currentLabel)  + " : i32");
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel - 1));
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
        node->elseBranch->accept(*this);
        bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel++));
}


void IRGen::visitWhileStatement(WhileStatementNode *node) {
    scopes.emplace();
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel));
    auto conditionLabel = currentLabel++;
    node->condition->accept(*this);
    auto endLabel = currentLabel++;
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF_FALSE) + " L" + std::to_string(endLabel) + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    for (const auto& n : node->body->statements) {
        n->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::JMP) + " L" + std::to_string(conditionLabel)  + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(endLabel));
    scopes.pop();
}

void IRGen::visitDoWhileStatement(DoWhileStatementNode *node) {
    scopes.emplace();
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel));
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    auto bodyLabel = currentLabel++;
    for (const auto& n : node->whileStatement->body->statements) {
        n->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    node->whileStatement->condition->accept(*this);
    auto endLabel = currentLabel++;
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF_FALSE) + " L" + std::to_string(endLabel) + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::JMP) + " L" + std::to_string(bodyLabel)  + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(endLabel));
    scopes.pop();
}

void IRGen::visitPrintln(PrintlnStatementNode *node) {
    node->expression->accept(*this);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::PRINT) + " [" + node->type + "]");
}


void IRGen::visitReturnStatement(ReturnStatementNode *node) {
    if (node->returnValue) {
        node->returnValue->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::RET)  + " [" + node->returnType + "]");
}



void IRGen::visitForStatement(ForStatementNode *node) {
    scopes.emplace();
    node->initializer->accept(*this);

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(currentLabel));

    auto conditionLabel = currentLabel++;
    auto endLabel = currentLabel++;

    node->condition->accept(*this);

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::IF_FALSE) + " L" + std::to_string(endLabel) + " : i32");

    node->body->accept(*this);

    node->posBody->accept(*this);

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::JMP) + " L" + std::to_string(conditionLabel)  + " : i32");

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LABEL) + " L" + std::to_string(endLabel));
    scopes.pop();
}
