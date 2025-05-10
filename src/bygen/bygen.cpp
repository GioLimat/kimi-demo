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

    if (symbolTable.size() == 1) {
        if (globalVariables.contains(name)) {
            return globalVariables[name];
        }    std::cout << symbolTable.size() << std::endl;
        throw std::runtime_error("Unknown identifier name " +name );
    }
    std::stack<std::unordered_map<std::string, uint32_t>> temp = symbolTable;
    while (!temp.empty()) {
        if (temp.top().contains(name)) {
            return temp.top()[name];
        }
        temp.pop();
    }
    if (globalVariables.contains(name)) {
        return globalVariables[name];
    }
    throw std::runtime_error("Unknown identifier name " +name );
}

void ByGen::declareIdentifier(const std::string &name) {
    if (symbolTable.size()  == 1) {
        globalVariables[name] = globalNextId++;
        return;
    }
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



void ByGen::emitBasedOnType(const std::string &type) {
    if (type == "i32") emitLiteralLE<int32_t>(0);
    else if (type == "f64") emitLiteralLE<double>(0.0);
    else emitLiteralLE<int32_t>(0);
}



std::vector<uint8_t>  ByGen::generate() {


    for (const auto& instruction : ir) {
        const auto parts  = splitBySpace(instruction);
        const auto& instructionType = parts[0];

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
            emitLiteralLE<int32_t>(static_cast<int32_t>(0x00));
            continue;
        }
        if (instructionType == "END_BLOCK") {
            symbolTable.pop();
            emitLiteralLE<int32_t>(static_cast<int32_t>(0x00));
            continue;
        }
        if (instructionType == "CONST") {
            if (type == "i32") {
                int32_t v = std::stoi(parts[1]);
                emitLiteralLE<int32_t>(v);
            } else if (type == "f64") {
                double v = std::stod(parts[1]);
                emitLiteralLE<double>(v);
            }
            continue;
        }

        if (instructionType == "STORE") {
            const std::string& name = parts[1];

            declareIdentifier(name);

            const uint32_t val = getIdentifierId(name);

            emitLiteralLE<int32_t>(val);
            continue;
        }

        if (instructionType == "LOAD" || instructionType == "GLOAD") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);

            if (type == "i32") emitLiteralLE<int32_t>(static_cast<int32_t>(varId));
            continue;
        }
        if (instructionType == "FN") {
            const std::string& fnName = parts[1] + "_FN";

            declareIdentifier(fnName);

            const uint32_t val = getIdentifierId(fnName);

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));
            continue;
        }
        if (instructionType == "FN_PARAM") {
            const std::string& paramName = parts[1];

            uint32_t varId = nextId++;
            symbolTable.top()[paramName] = varId;

            if (type == "i32") emitLiteralLE<int32_t>(static_cast<int32_t>(varId));
            if (type == "f64") emitLiteralLE<double>(varId);
            continue;
        }
        if (instructionType == "CALL") {
            const std::string& fnName = parts[1] + "_FN";

            const uint32_t val = getIdentifierId(fnName);

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));

            continue;
        }

        emitBasedOnType(type);
    }

    return bytecode;
}
