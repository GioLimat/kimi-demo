//
// Created by home on 02/05/25.
//

#include "bygen_instructions.h"
#include "bygen.h"

#include <iostream>
#include <ranges>
#include <sstream>
#include <bits/ostream.tcc>

ByGen::ByGen(std::vector<std::string> ir) : ir(std::move(ir)) {
    symbolTable.emplace();
}


std::vector<std::string> ByGen::splitBySpace(const std::string& str) const {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

uint32_t ByGen::getIdentifierId(const std::string &name) {
    std::stack<std::unordered_map<std::string, uint32_t>> temp = symbolTable;
    while (!temp.empty()) {
        if (temp.top().contains(name)) {
            return temp.top()[name];
        }
        temp.pop();
    }
    throw std::runtime_error("Unknown identifier name " +name );
}

void ByGen::declareIdentifier(const std::string &name) {
    uint32_t varId;
    std::stack<std::unordered_map<std::string, uint32_t>> temp = symbolTable;
    bool found = false;
    while (!temp.empty()) {
        auto& scope = temp.top();
        auto it = scope.find(name);
        if (it != scope.end()) {
            varId = it->second;
            found = true;
            break;
        }
        temp.pop();
    }
    if (!found) {
        auto& currentScope = symbolTable.top();
        varId = nextId++;
        currentScope[name] = varId;
    }
}



void ByGen::pushULEB128Identifier(uint32_t val) {
    while (val >= 0x80) {
        bytecode.push_back(static_cast<uint8_t>((val & 0x7F) | 0x80));
        val >>= 7;
    }
    bytecode.push_back(static_cast<uint8_t>(val));
}


std::vector<uint8_t>  ByGen::generate() {


    for (const auto& instruction : ir) {
        const auto parts  = splitBySpace(instruction);
        const auto& instructionType = parts[0];
        // Leave the current iteration and crate or destroy a scope


        bytecode.push_back(ByMapper::getInstruction(instructionType));
        bytecode.push_back(0x00);

        std::string type;

        for (int i = 0; i < parts.size(); ++i) {
            if (parts[i] == ":") {
                type = parts[i + 1];
            }
        }
        bytecode.push_back(ByMapper::getType(type));


        if (instructionType == "INIT_BLOCK") {
            symbolTable.emplace();
            bytecode.push_back(0x00);
            continue;
        }
        if (instructionType == "END_BLOCK") {
            symbolTable.pop();
            bytecode.push_back(0x00);
            continue;
        }
        if (instructionType == "CONST") {
            uint32_t val = std::stoul(parts[1]);
            while (val >= 0x80) {
                bytecode.push_back(static_cast<uint8_t>((val & 0x7F) | 0x80));
                val >>= 7;
            }
            bytecode.push_back(static_cast<uint8_t>(val));
            continue;
        }

        if (instructionType == "STORE") {
            const std::string& name = parts[1];

            declareIdentifier(name);

            const uint32_t val = getIdentifierId(name);

            pushULEB128Identifier(val);

            continue;
        }

        if (instructionType == "LOAD") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);

            pushULEB128Identifier(varId);

            continue;
        }
        if (instructionType == "FN") {
            const std::string& fnName = parts[1] + "_FN";

            declareIdentifier(fnName);

            const uint32_t val = getIdentifierId(fnName);

            pushULEB128Identifier(val);
            continue;
        }
        if (instructionType == "FN_PARAM") {
            const std::string& paramName = parts[1];

            symbolTable.top()[paramName] = nextId++ - 1;

            pushULEB128Identifier(symbolTable.top()[paramName]);
            continue;
        }
        if (instructionType == "CALL") {
            const std::string& fnName = parts[1] + "_FN";

            const uint32_t val = getIdentifierId(fnName);

            pushULEB128Identifier(val);

            continue;
        }


        bytecode.push_back(0x00);
    }

    return bytecode;
}
