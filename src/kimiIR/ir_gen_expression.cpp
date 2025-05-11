//
// Created by home on 19/04/25.
//

#include "ir_gen.h"
#include "ir_instructions.h"
#include "type_analyzer.h"


void IRGen::visitBinaryExpr(BinaryExprNode *expression) {
    auto bin = expression;
    TypeInfer::analyzeExpression(bin, &scopes);
    bin->left->accept(*this);
    bin->right->accept(*this);
    bytecode.push_back(IRMapper::getOperator(bin->op) + " : " + bin->type);
}

void IRGen::visitNumber(NumberNode *number) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CONST) + " " + number->value + " : " + number->type);
}



void IRGen::visitIdentifier(IdentifierExprNode *identifier) {
    auto it = lookup<SemanticAnalyzer::VariableInfo>(identifier->name, "Variable not found: " + identifier->name);
    if (it.isGlobal) bytecode.push_back(IRMapper::getInstruction(IRInstruction::GLOAD) + " " + identifier->name + " : " + "i32");
    else bytecode.push_back(IRMapper::getInstruction(IRInstruction::LOAD) + " " + identifier->name + " : " + "i32");
}


void IRGen::visitCallFunction(CallFunctionNode *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CALL) + " " + node->name + " " + ": i32");
    for (const auto &arg : node->arguments) {
        arg->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CALL_END) + " " + node->name);
}


void IRGen::visitGenericExpressionNode(GenericExpressionNode *node) {
    node->node->accept(*this);
}
