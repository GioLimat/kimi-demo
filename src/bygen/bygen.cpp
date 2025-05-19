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

void ByGen::declareIdentifier(const std::string &name, bool isParam = false) {
    uint32_t varId;
    std::stack<std::unordered_map<std::string, uint32_t>> temp = symbolTable;

    auto& currentScope = symbolTable.top();

    if (currentScope.contains(name)) return;

    if (isParam) {
        varId = currentScope.size();
    } else {
        varId = nextId++;
    }

    currentScope[name] = varId;
}



void ByGen::emitBasedOnType(const std::string &type) {
    if (type == "i8") emitLiteralLE<int8_t>(0);
    else if (type == "i16") emitLiteralLE<int16_t>(0);
    else if (type == "i32") emitLiteralLE<int32_t>(0);
    else if (type == "f64") emitLiteralLE<double>(0.0);
    else if (type == "bool") emitLiteralLE<bool>(false);
}



std::vector<uint8_t> ByGen::generate() {

    size_t i = -1;
    for (const auto& instruction : ir) {
        i++;
        const auto parts  = splitBySpace(instruction);
        const auto& instructionType = parts[0];

        if (instructionType == "LABEL") continue;

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
            continue;
        }
        if (instructionType == "END_BLOCK") {
            symbolTable.pop();
            continue;
        }

        if (instructionType == "CONST") {
            if (type == "i8") {
                int8_t v = std::stoi(parts[1]);
                emitLiteralLE<int8_t>(v);
            } else if (type == "i16") {
                int16_t v = std::stoi(parts[1]);
                emitLiteralLE<int16_t>(v);
            }
            else if (type == "i32") {
                int32_t v = std::stoi(parts[1]);
                emitLiteralLE<int32_t>(v);
            } else if (type == "f64") {
                double v = std::stod(parts[1]);
                emitLiteralLE<double>(v);
            }
            else if (type == "bool") {
                int32_t v = std::stoi(parts[1]);
                emitLiteralLE<uint8_t>(v);
            }
            continue;
        }

        if (instructionType == "STORE") {
            const std::string& name = parts[1];

            try {
                getIdentifierId(name);
            }
            catch (...) {
                declareIdentifier(name);
            }

            const uint32_t val = getIdentifierId(name);

            emitLiteralLE<int32_t>(val);
            continue;
        }

        if (instructionType == "INC" ||
            instructionType == "DEC" ||
            instructionType == "POST_INC" ||
            instructionType == "POST_DEC") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);

            emitLiteralLE<int32_t>(varId);

            continue;
        }

        if (instructionType == "LOAD") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);

            emitLiteralLE<int32_t>(static_cast<int32_t>(varId));
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

            declareIdentifier(paramName, true);

            const uint32_t val = getIdentifierId(paramName);

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));
            continue;
        }
        if (instructionType == "CALL") {
            const std::string& fnName = parts[1] + "_FN";

            const uint32_t val = getIdentifierId(fnName);

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));
            continue;
        }
        if (instructionType == "IF_FALSE" || instructionType == "JMP") {
            const auto& label = parts[1];
            const auto labelId = label.substr(1, label.size());

            int64_t offset = 0;


            // SEARCHING AT FRONT OF THE IF OR JMP
            size_t tempI = i + 1;
            bool foundUpFront = false;


            while (tempI < ir.size()) {
                auto tempLabel = splitBySpace(ir[tempI]);
                if (tempLabel[0] == "LABEL") {
                    if (tempLabel[1] == label) {
                        foundUpFront = true;
                        break;
                    }
                    tempI++;
                    continue;
                }
                const auto tempParts = splitBySpace(ir[tempI]);
                std::string tempType;

                for (size_t j = 0; j < tempParts.size(); ++j) {
                    if (tempParts[j] == ":") {
                        if (!tempParts[j + 1].empty()) {
                            tempType = tempParts[j + 1];
                        }
                    }
                }
                offset += 3;
                if (tempType == "i8") offset += 1;
                else if (tempType == "i16") offset += 2;
                else if (tempType == "i32") offset += 4;
                else if (tempType == "f64") offset += 8;
                else if (tempType == "bool") offset += 1;
                tempI++;
            }


            // SEARCHING AT BACK OF THE IF OR JMP
            if (!foundUpFront && static_cast<int64_t>(i) - 1  >= 0) {
                offset = 0;
                tempI = i - 1;
                while (true) {
                    auto tempLabel = splitBySpace(ir[tempI]);
                    if (tempLabel[0] == "LABEL") {
                        if (tempLabel[1] == label) {
                            break;
                        }
                        tempI--;
                        continue;
                    }
                    const auto tempParts = splitBySpace(ir[tempI]);
                    std::string tempType;


                    for (size_t j = 0; j < tempParts.size(); ++j) {
                        if (tempParts[j] == ":") {
                            if (!tempParts[j + 1].empty()) {
                                tempType = tempParts[j + 1];
                            }
                        }
                    }
                    offset -= 3;
                    if (tempType == "i8") offset -= 1;
                    else if (tempType == "i16") offset -= 2;
                    if (tempType == "i32") offset -= 4;
                    else if (tempType == "f64") offset -= 8;
                    else if (tempType == "bool") offset -= 1;
                    tempI--;
                 }
                offset -= 7; // This is the amount of bytes in the jmp, that is not considered at the count above
                }
            emitLiteralLE<int32_t>(offset);
            continue;
        }

        emitBasedOnType(type);
    }

    return bytecode;
}
