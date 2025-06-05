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
    symbolTable.emplace_back();
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



std::vector<uint8_t> ByGen::generate() {

    size_t i = -1;
    for (auto& instruction : ir) {
        i++;
        const auto parts  = splitBySpace(instruction);
        const auto& instructionType = parts[0];

        if (instructionType == "LABEL") continue;

        if (instructionType != "CONST_STR") bytecode.push_back(ByMapper::getInstruction(instructionType));

        std::string type = getType(parts);

        if (twoLengthInstruction(instructionType)) {
            emmitMeta(instruction); // the last meta if is two-length instruction, is a type
            continue;
        }


        if (instructionType == "INIT_BLOCK") {
            symbolTable.emplace_back();
            continue;
        }
        if (instructionType == "END_FN") {
            symbolTable.pop_back();
            idFnTable.pop_back();
            continue;
        }
        if (instructionType == "END_BLOCK") {
            if (instruction.find("ignorable") != std::string::npos) continue;
            symbolTable.pop_back();
            continue;
        }

        if (instructionType == "CONST_STR") {
            std::string value = instruction.substr(10,  std::stoi(getMeta(instruction)));

            uint64_t len = value.size();

            // iF the string is less equal than 7 bytes, we need to emit it as a 64-bit integer to store at the stack instead of the heap
            if (value.size() <= 8) {
                uint64_t raw = 0;
                for (size_t i = 0; i < value.size(); ++i) {
                    raw |= (static_cast<uint64_t>(static_cast<uint8_t>(value[i])) << (8 * i));
                }
                bytecode.push_back(ByMapper::getInstruction("CONST"));
                bytecode.push_back(0x06);
                emitLiteralLE<uint64_t>(raw);
            }
            continue;
        }

        if (instructionType == "CONST") {
            emitLiteralLE<uint8_t>(ByMapper::getType(type));

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
            }
            else if (type == "i64") {
                int64_t v = std::stoll(parts[1]);
                emitLiteralLE<int64_t>(v);
            }
            else if (type == "f32") {
                float v = std::stof(parts[1]);
                emitLiteralLE<float>(v);
            }
            else if (type == "f64") {
                double v = std::stod(parts[1]);
                emitLiteralLE<double>(v);
            }
            else if (type == "bool") {
                int32_t v = std::stoi(parts[1]);
                emitLiteralLE<uint8_t>(v);
            }
            else if (type == "char") {
                uint32_t v = std::stoi(parts[1]);
                emitLiteralLE<uint32_t>(v);
            }
            continue;
        }

        if (instructionType == "STORE") {
            const std::string& name = parts[1];

            const auto isDeclaration = instruction.find("declaration") != std::string::npos;


            if (isDeclaration) {
                declareIdentifier(name, false, false);
            }

            const uint32_t val = getIdentifierId(name);

            emitLiteralLE<uint8_t>(jumpsToFindLocal);
            emitLiteralLE<uint8_t>(scopesToFindLocal);

            jumpsToFindLocal = 0;
            scopesToFindLocal = 0;

            emitLiteralLE<int32_t>(val);
            continue;
        }

        if (instructionType == "INC" ||
            instructionType == "DEC" ||
            instructionType == "POST_INC" ||
            instructionType == "POST_DEC") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);

            emitLiteralLE<uint8_t>(jumpsToFindLocal);
            emitLiteralLE<uint8_t>(scopesToFindLocal);

            jumpsToFindLocal = 0;
            scopesToFindLocal = 0;

            emitLiteralLE<int32_t>(varId);

            continue;
        }

        if (instructionType == "LOAD") {
            const std::string& varName = parts[1];

            const uint32_t varId = getIdentifierId(varName);




            emitLiteralLE<uint8_t>(jumpsToFindLocal);
            emitLiteralLE<uint8_t>(scopesToFindLocal);

            jumpsToFindLocal = 0;
            scopesToFindLocal = 0;

            emitLiteralLE<int32_t>(static_cast<int32_t>(varId));
            continue;
        }
        if (instructionType == "FN") {
            const std::string& fnName = parts[1] + "_FN";

            declareIdentifier(fnName, false, true);
            symbolTable.emplace_back();
            idFnTable.push_back(symbolTable.size() - 1);

            const uint32_t val = getIdentifierId(fnName);

            scopesToFindLocal = 0;
            jumpsToFindLocal = 0;

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));
            continue;
        }
        if (instructionType == "FN_PARAM") {
            const std::string& paramName = parts[1];

            declareIdentifier(paramName, true, false);

            const uint32_t val = getIdentifierId(paramName);

            jumpsToFindLocal = 0;
            scopesToFindLocal = 0;

            emitLiteralLE<int32_t>(static_cast<int32_t>(val));
            continue;
        }
        if (instructionType == "CALL") {
            const std::string& fnName = parts[1] + "_FN";

            const uint32_t val = getIdentifierId(fnName);

            jumpsToFindLocal = 0;
            scopesToFindLocal = 0;

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
                std::string tempType = getType(tempParts);

                const auto len = getInstructionLength(tempParts[0], tempType, ir[tempI]);
                //std::cout << tempParts[0] << " : " << tempType << " len " << len << std::endl;
                offset += len;
                tempI++;
            }



            // SEARCHING AT BACK OF THE JMP
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
                    std::string tempType = getType(tempParts);

                    const auto len = getInstructionLength(tempParts[0], tempType, ir[tempI]);
                    //std::cout << tempParts[0] << " : " << tempType << " len " << len << std::endl;
                    offset -= len;
                    tempI--;
                 }
                offset -= 1 + 4;// This is the amount of bytes in the jmp, that is not considered at the count above
                }
            emitLiteralLE<int32_t>(offset);
            continue;
        }

    }
    bytecode.push_back(0x00); // HALT
    return bytecode;
}
