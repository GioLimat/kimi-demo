//
// Created by home on 19/04/25.
//

#include <iostream>

#include "ir_gen.h"
#include "ir_instructions.h"
#include "type_analyzer.h"


void IRGen::visitBinaryExpr(BinaryExprNode *expression) {
    auto bin = expression;
    TypeInfer::analyzeExpression(bin, &scopes);
    bin->left->accept(*this);
    bin->right->accept(*this);
    bytecode.push_back(IRMapper::getOperator(bin->op) + " ["  + bin->type + "]");
}

void IRGen::visitNumber(NumberNode *number) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CONST) + " " + number->value + " : " + number->type);
}



void IRGen::visitIdentifier(IdentifierExprNode *identifier) {
    auto it = lookup<SemanticAnalyzer::VariableInfo>(identifier->name, "Variable not found: " + identifier->name);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::LOAD) + " " + identifier->name + " : " + it.type +  " " +
        + " [" + it.type + "]"
    );
}


void IRGen::visitCallFunction(CallFunctionNode *node) {
    for (const auto &arg : node->arguments) {
        arg->accept(*this);
    }
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CALL) + " " + node->name + " " + ": i32");
}


void IRGen::visitUnaryExpr(UnaryExprNode *node) {
    node->operand->accept(*this);
    if (node->op == "-") bytecode.push_back(IRMapper::getInstruction(IRInstruction::NEG)  + " [" + node->type + "]");
    else if (node->op == "--" || node->op == "++") {
        auto ident = dynamic_cast<IdentifierExprNode*>(node->operand.get());
        if (ident == nullptr) throw std::runtime_error("Undefined identifier");
        auto variable = lookup<SemanticAnalyzer::VariableInfo>(ident->name, "Variable not found: " + ident->name);
        if (variable.isConst) {
            throw std::runtime_error("Cannot modify a constant variable: " + ident->name);
        }
        if (node->op == "--") bytecode.push_back(IRMapper::getInstruction(IRInstruction::DEC) + " " + ident->name + " : " + node->type);
        else bytecode.push_back(IRMapper::getInstruction(IRInstruction::INC) + " " + ident->name + " : " + node->type);
    }
    else bytecode.push_back(IRMapper::getOperator(node->op) + " [" + node->type + "]");
}


void IRGen::visitAssignmentExpr(AssignmentExprNode *node) {
    node->value->accept(*this);
    auto it = lookup<SemanticAnalyzer::VariableInfo>(node->name, "Variable not found: " + node->name);
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::STORE) + " " + node->name + " : " + "i32" +
        " [" + it.type + "]"
    );
}


void IRGen::visitGenericExpressionNode(GenericExpressionNode *node) {
    node->node->accept(*this);
}


void IRGen::visitPostFixExpr(PostFixExprNode *node) {
    node->operand->accept(*this);
    auto ident = dynamic_cast<IdentifierExprNode*>(node->operand.get());
    auto variable = lookup<SemanticAnalyzer::VariableInfo>(ident->name, "Variable not found: " + ident->name);
    if (variable.isConst) {
        throw std::runtime_error("Cannot modify a constant variable: " + ident->name);
    }
    if (node->op == "--") bytecode.push_back(IRMapper::getInstruction(IRInstruction::POST_DEC) + " " + ident->name + " : " + node->type);
    else if (node->op == "++") bytecode.push_back(IRMapper::getInstruction(IRInstruction::POST_INC) + " " + ident->name + " : " + node->type);
}


void IRGen::visitBoolean(BooleanNode *node) {
    std::string value = node->value ? "1" : "0";
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CONST) + " " + value + " : " + "bool");
}


void IRGen::visitCharLiteralExpr(CharLiteralExpr *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::CONST) + " " + std::to_string(node->code) + " : " + "char");
}

void IRGen::visitStringLiteralExpr(StringLiteralExpr *node) {
    bytecode.push_back(IRMapper::getInstruction(IRInstruction::ALLOC) +
       " '" + node->value + "'"
       + " : str "
        + " [" + std::to_string(node->value.length()) + + ", " + std::to_string(node->value.size()) + "]");
}

void IRGen::visitIndexAccessExpr(IndexAccessExpr *node) {
    node->base->accept(*this);

    node->index->accept(*this);

    std::string type = TypeInfer::analyzeExpression(node->base.get(), &scopes);

    bytecode.push_back(IRMapper::getInstruction(IRInstruction::INDEX_ACCESS) + " : " + type  + " [" +
        (node->generateHeapValue ? "heap" : "value") + ", " + type
    +  "]");
}
