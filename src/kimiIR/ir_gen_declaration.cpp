//
// Created by home on 19/04/25.
//

#include <iostream>

#include "ir_gen_expression.h"
#include "ir_gen_declaration.h"
#include "ir_instructions.h"

std::vector<std::string> IRGenDeclaration::varDeclaration(const VarDeclarationNode *varDeclaration) {
    std::vector<std::string> bytecode;

    if (varDeclaration->initializer) {
        std::vector<std::string> expr = IRGenExpression::generateExpression(varDeclaration->initializer.get());
        bytecode.insert(bytecode.end(), expr.begin(), expr.end());
    }

    auto instruction = IRMapper::getInstruction(IRInstruction::STORE) + " : i32 ";
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

    bytecode.push_back(instruction);

    return bytecode;
}


std::vector<std::string> IRGenDeclaration::generateDeclaration(const StatementNode* statement) {
    std::vector<std::string> bytecode;

    if (const auto var = dynamic_cast<const VarDeclarationNode*>(statement)) {
        const auto decl = varDeclaration(var);
        if (!decl.empty()) bytecode.insert(bytecode.end(), decl.begin(), decl.end());
    }

    return bytecode;
}