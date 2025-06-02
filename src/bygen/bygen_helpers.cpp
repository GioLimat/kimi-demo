//
// Created by home on 02/06/25.
//

#include <iostream>
#include <ostream>

#include "bygen.h"
#include "bygen_instructions.h"

uint32_t ByGen::getIdentifierId(const std::string &name) {

    SymbolTableType temp = symbolTable;

    uint32_t id = symbolTable.size() - 1;

    while (!temp.empty()) {
        if (temp.back().contains(name)) {
            return temp.back()[name];
        }
        if (std::find(idFnTable.begin(), idFnTable.end(), id) != idFnTable.end()) {
            jumpsToFindLocal ++;
            scopesToFindLocal = 0;
        }
        else {
            scopesToFindLocal ++;
        }
        temp.pop_back();
        id--;
    }
    throw std::runtime_error("Unknown identifier name " +name );
}

void ByGen::declareIdentifier(const std::string &name, bool isParam = false, bool isFunction = false) {
    uint32_t varId;
    SymbolTableType temp = symbolTable;

    auto& currentScope = symbolTable.back();

    if (currentScope.contains(name)) return;

    if (isParam) {
        varId = currentScope.size();
    }
    else if (isFunction) {
        varId = functionId++;
    }
    else {
        varId = nextId++;
    }

    currentScope[name] = varId;
}



void ByGen::emitBasedOnType(const std::string &type) {
    if (type == "i8") emitLiteralLE<int8_t>(0);
    else if (type == "i16") emitLiteralLE<int16_t>(0);
    else if (type == "i32") emitLiteralLE<int32_t>(0);
    else if (type == "i64") emitLiteralLE<int64_t>(0);
    else if (type == "f32") emitLiteralLE<float>(0);
    else if (type == "f64") emitLiteralLE<double>(0.0);
    else if (type == "bool") emitLiteralLE<bool>(false);
    else if (type == "char") emitLiteralLE<uint32_t>(0);
}






void ByGen::getMeta(const std::string &instruction)  {
    if (instruction.find('[') != std::string::npos) {
        std::string meta;
        size_t storeTypeI = instruction.find('[') + 1;
        while (instruction[storeTypeI] != ']') {
            meta += instruction[storeTypeI];
            storeTypeI++;
        }

        if (instruction.find(", ") != std::string::npos) {
           auto end =  instruction.find(", ") + 2;
            meta = instruction.substr(end, storeTypeI);
        }
        bytecode.push_back(ByMapper::getType(meta));
    }
}


std::string ByGen::getType(const std::vector<std::string>& parts) {
    std::string type;

    for (int i = 0; i < parts.size(); ++i) {
        if (parts[i] == ":") {
            type = parts[i + 1];
        }
    }
    return  type;
}



bool ByGen::twoLengthInstruction(const std::string &instruction) {
    return instruction == "ADD" ||
           instruction == "SUB" ||
           instruction == "MUL" ||
           instruction == "DIV" ||
           instruction == "MOD" ||

           instruction ==  "GREATER" ||
           instruction ==  "LESS" ||
            instruction ==  "EQUAL" ||
            instruction ==  "GREATER_EQUAL" ||
            instruction ==  "LESS_EQUAL" ||
            instruction ==  "NOT_EQUAL" ||

            instruction == "NOT" ||
            instruction == "NEG" ||


            instruction ==  "AND" ||
            instruction == "OR" ||
            instruction == "RET" ||

            instruction == "BIT_AND"  ||
            instruction == "BIT_OR"  ||
            instruction == "BIT_XOR"   ||
            instruction == "SHIFT_LEFT"  ||
            instruction == "SHIFT_RIGHT"   ||
            instruction == "BIT_NOT"  ||

           instruction == "PRINT";
}


bool ByGen::sevenLengthInstruction(const std::string &instruction) {
    return instruction == "LOAD" ||
           instruction == "INC" ||
           instruction == "DEC" ||
           instruction == "POST_INC" ||
           instruction == "POST_DEC";
}


uint64_t ByGen::getInstructionLength(const std::string& instruction, const std::string& tempType = "") {
    uint64_t offset = 0;

    if (twoLengthInstruction(instruction)) {
        offset = 2;
    }
    else if (instruction == "CONST") {
        offset = 2 + ByMapper::getType(tempType) / 8;
    }
    else if (sevenLengthInstruction(instruction)) {
        offset += 3 + 4;
    }
    else if (instruction == "END_BLOCK" || instruction == "INIT_BLOCK" || instruction == "END_FN") {
        offset += 1;
    }
    else if (
        instruction == "IF_FALSE"
        || instruction == "JMP"
        || instruction == "FN"
        || instruction == "FN_PARAM"
        || instruction == "CALL"
        || instruction == "STORE") {
        offset += 1 + 4;
        }
    else {
        throw std::runtime_error("Unknown instruction " + instruction);
    }
    return offset;
}
