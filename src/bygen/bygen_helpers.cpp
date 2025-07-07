//
// Created by home on 02/06/25.
//

#include <iostream>
#include <ostream>

#include "bygen.h"
#include "bygen_instructions.h"
#include "string_header.h++"

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






static inline std::string trim(const std::string &s) {
    auto l = s.find_first_not_of(" \t\n\r");
    auto r = s.find_last_not_of(" \t\n\r");
    return (l == std::string::npos)
         ? std::string{}
    : s.substr(l, r - l + 1);
}

void ByGen::emmitMeta(const std::string &instruction) {
    if (instruction.find('[') != std::string::npos){
        size_t left  = instruction.find('[');
        size_t right = instruction.find(']');
        if (left == std::string::npos || right == std::string::npos || right <= left)
            return;

        std::string meta = instruction.substr(left + 1, right - left - 1);
        meta = trim(meta);

        auto pos = meta.rfind(',');
        if (pos != std::string::npos)
            meta = trim(meta.substr(pos + 1));

        bytecode.push_back(ByMapper::getType(meta));
}
}

std::string ByGen::getMeta(const std::string &instruction)  {
    if (instruction.find('[') != std::string::npos) {
        size_t left  = instruction.find('[');
        size_t right = instruction.find(']');
        if (left == std::string::npos || right == std::string::npos || right <= left)
            return "";

        std::string meta = instruction.substr(left + 1, right - left - 1);
        meta = trim(meta);

        auto pos = meta.rfind(',');
        if (pos != std::string::npos)
            meta = trim(meta.substr(pos + 1));

        return meta;
    }
}

std::string ByGen::getFirstMeta(const std::string &instruction) {
    size_t start = instruction.find('[');
    size_t end = instruction.find(']');

    if (start == std::string::npos || end == std::string::npos || start + 1 >= end) {
        return "";
    }

    std::string insideBrackets = instruction.substr(start + 1, end - start - 1);
    size_t commaPos = insideBrackets.find(',');

    if (commaPos != std::string::npos) {
        return insideBrackets.substr(0, commaPos);
    } else {
        return insideBrackets;
    }
}


std::string ByGen::extractStringLiteral(const std::string &instruction) {
    size_t firstQuote = instruction.find('\'');
    size_t secondQuote = instruction.find('\'', firstQuote + 1);

    if (firstQuote == std::string::npos || secondQuote == std::string::npos || secondQuote <= firstQuote) {
        return "";
    }

    return instruction.substr(firstQuote + 1, secondQuote - firstQuote - 1);
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

            instruction == "INDEX_ACCESS" ||

           instruction == "PRINT";
}


bool ByGen::sevenLengthInstruction(const std::string &instruction) {
    return instruction == "LOAD" ||
           instruction == "INC" ||
           instruction == "DEC" ||
           instruction == "POST_INC" ||
           instruction == "POST_DEC"
    || instruction == "STORE";
}


uint64_t ByGen::getInstructionLength(const std::string& instruction, const std::string& tempType = "", const std::string& fullInstruction ) {
    uint64_t offset = 0;

    if (twoLengthInstruction(instruction)) {
        offset = 2;
    }
    else if (instruction == "CONST") {
        offset = 2 + ByMapper::getSize(tempType) / 8;
    }
    else if (instruction == "CONST_STR") {
        offset = 2 + 8;
    }
    else if (instruction == "ALLOC") {
        offset = 10 + std::stoll(getMeta(fullInstruction)) / 8;
    }
    else if (sevenLengthInstruction(instruction)) {
        offset += 3 + 4;
    }
    else if (instruction == "END_BLOCK" || instruction == "INIT_BLOCK" || instruction == "END_FN") {
        offset += 1;
    }
    else if (    instruction == "FN"
        || instruction == "FN_PARAM"
        || instruction == "CALL" ||
        instruction == "IF_FALSE"
        || instruction == "JMP") {
        offset += 1 + 4;
    }
    else {
        throw std::runtime_error("Unknown instruction " + instruction);
    }
    return offset;
}


void ByGen::emitLiteralLE(uint64_t value)  {
    auto* bytes = reinterpret_cast<uint8_t*>(&value);
    for (int i = 0; i < 8; i++) {
        bytecode.push_back(bytes[i]);
    }
}
