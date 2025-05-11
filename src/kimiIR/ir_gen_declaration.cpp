//
// Created by home on 19/04/25.
//

#include <iostream>

#include "ir_gen.h"
#include "ir_instructions.h"

void IRGen::visitVarDeclaration(VarDeclarationNode *varDeclaration) {

    if (varDeclaration->initializer) {
       varDeclaration->initializer->accept(*this);
    }

    auto instruction = IRMapper::getInstruction(IRInstruction::STORE) + " " + varDeclaration->name + " : " + "i32" + " ";
    std::string meta;

    if (varDeclaration->isConst) {
        meta = IRMapper::getMeta(IRMeta::CONST) + ", ";
    }else {
        meta = IRMapper::getMeta(IRMeta::MUT) + ", ";
    }

    if (!meta.empty()) {
        meta = "[" + meta;
        meta = meta.substr(0, meta.size() - 2);
        meta += "]";
        instruction += meta;
    }

    scopes.top()[varDeclaration->name] = SemanticAnalyzer::VariableInfo{varDeclaration->type, varDeclaration->isConst, varDeclaration->isGlobal};

    bytecode.push_back(instruction);
}


void IRGen::visitFunctionDeclaration(FunctionDeclarationNode *function) {
    scopes.emplace();
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::FN) + " " + function->name + " : i32");
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INIT_BLOCK));
    for (const auto &param : function->parameters) {
        auto instruction = IRMapper::getInstruction(IRInstruction::FN_PARAM) + " " + param.name + " : " + param.type;
        bytecode.push_back(instruction);
        scopes.top()[param.name] = SemanticAnalyzer::VariableInfo{param.type, false, false};
    }
    for (const auto &node : function->body->statements) {
        node->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::END_BLOCK));
    scopes.pop();
}