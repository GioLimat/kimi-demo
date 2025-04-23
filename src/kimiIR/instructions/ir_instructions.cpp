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
    return "";
}


std::string IRMapper::getMeta(const IRMeta meta) {
    switch (meta) {
        case IRMeta::CONST: return "const";
        case IRMeta::MUT: return "mut";
        case IRMeta::GLOBAL: return "global";
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
    }
    return "";
}


std::string IRMapper::getInstruction(const IRInstruction ir) {
    switch (ir) {
        case IRInstruction::CONST: return "CONST";
        case IRInstruction::LOAD: return "LOAD";
        case IRInstruction::STORE: return "STORE";
        case IRInstruction::PRINT: return "PRINT";
        default: return "";
    }
}
