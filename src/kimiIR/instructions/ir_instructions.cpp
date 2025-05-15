//
// Created by home on 22/04/25.
//

#include "ir_instructions.h"

std::string IRMapper::getOperator(const std::string &op) {
    if (op == "+") return "ADD";
    if (op == "-") return "SUB";
    if (op == "*") return "MUL";
    if (op == "/") return "DIV";
    if (op == "%") return "MOD";
    if (op == ">") return "GREATER";
    if (op == "<") return "LESS";
    if (op == "==") return "EQUAL";
    if (op == "!=") return "NOT_EQUAL";
    if (op == "&&") return "AND";
    if (op == "||") return "OR";
    if (op == "!") return "NOT";
    if (op == ">=") return    "GREATER_EQUAL";
    if (op == "<=") return "LESS_EQUAL";
    return "";
}


std::string IRMapper::getMeta(const IRMeta meta) {
    switch (meta) {
        case IRMeta::CONST: return "const";
        case IRMeta::MUT: return "mut";
    }
    return "";
}


std::string IRMapper::getType(const IRType type) {
    switch (type) {
        case IRType::INT8: return "i8";
        case IRType::INT32: return "i32";
        case IRType::INT64: return "i64";
        case IRType::FLOAT32: return "f32";
        case IRType::FLOAT64: return "f64";
        case IRType::BOOL: return "bool";
        default: return "";
    }

}


std::string IRMapper::getInstruction(const IRInstruction ir) {
    switch (ir) {
        case IRInstruction::CONST: return "CONST";
        case IRInstruction::LOAD: return "LOAD";
        case IRInstruction::STORE: return "STORE";
        case IRInstruction::PRINT: return "PRINT";
        case IRInstruction::FN: return "FN";
        case IRInstruction::FN_PARAM: return "FN_PARAM";
        case IRInstruction::END_BLOCK: return "END_BLOCK";
        case IRInstruction::INIT_BLOCK: return "INIT_BLOCK";
        case IRInstruction::DO: return "DO";
        case IRInstruction::WHILE: return "WHILE";
        case IRInstruction::END_CONDITION: return "END_CONDITION";
        case IRInstruction::CALL: return "CALL";
        case IRInstruction::CALL_END: return "CALL_END";
        case IRInstruction::RET: return "RET";
        case IRInstruction::GLOAD: return "GLOAD";
        case IRInstruction::IF_FALSE: return "IF_FALSE";
        case IRInstruction::JMP: return "JMP";
        case IRInstruction::LABEL: return "LABEL";
        case IRInstruction::NEG: return "NEG";
        case IRInstruction::INC:  return "INC";
        case IRInstruction::DEC:  return "DEC";
        case IRInstruction::POST_INC: return "POST_INC";
        case IRInstruction::POST_DEC:  return "POST_DEC";
        default: return "";
    }
}
