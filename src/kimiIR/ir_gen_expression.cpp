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


void IRGen::visitUnaryExpr(UnaryExprNode *node) {
    node->operand->accept(*this);
    if (node->op == "-") bytecode.push_back(IRMapper::getInstruction(IRInstruction::NEG) + " : " + node->type);
    else if (node->op == "--" || node->op == "++") {
        auto ident = dynamic_cast<IdentifierExprNode*>(node->operand.get());
        if (ident == nullptr) throw std::runtime_error("Undefined identifier");
        if (node->op == "--") bytecode.push_back(IRMapper::getInstruction(IRInstruction::DEC) + " " + ident->name + " : " + node->type);
        else bytecode.push_back(IRMapper::getInstruction(IRInstruction::INC) + " " + ident->name + " : " + node->type);
    }
    else bytecode.push_back(IRMapper::getOperator(node->op) + " : " + node->type);
}


void IRGen::visitAssignmentExpr(AssignmentExprNode *node) {
    node->value->accept(*this);
    auto it = lookup<SemanticAnalyzer::VariableInfo>(node->name, "Variable not found: " + node->name);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::STORE) + " " + node->name + " : " + node->type);
}


void IRGen::visitGenericExpressionNode(GenericExpressionNode *node) {
    node->node->accept(*this);
}


