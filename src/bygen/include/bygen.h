//
// Created by home on 02/05/25.
//

#ifndef BYGEN_H
#define BYGEN_H
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>

class ByGen {
    using SymbolTableType = std::vector<std::unordered_map<std::string, uint32_t>>;
    SymbolTableType symbolTable;

    std::vector<uint64_t> idFnTable = {0};

    size_t nextId = 0;
    size_t functionId = 0;

    std::vector<uint8_t> bytecode;

    std::vector<std::string> ir;

    [[nodiscard]] std::vector<std::string> splitBySpace(const std::string& str) const;

    uint32_t getIdentifierId(const std::string& name);

    void declareIdentifier(const std::string& name, bool isParam, bool isFunction);


    size_t jumpsToFindLocal = 0;
    size_t scopesToFindLocal = 0;

    void emitBasedOnType(const std::string& type);
    uint64_t getInstructionLength(const std::string& instruction, const std::string& tempType, const std::string& fullInstruction = "");


    void emmitMeta(const std::string& instruction);
    std::string getMeta(const std::string& instruction);

    static std::string getType(const std::vector<std::string>& parts);


    static bool twoLengthInstruction(const std::string& instruction);
    static bool sevenLengthInstruction(const std::string& instruction);

    template<typename T>
    void emitLiteralLE(const T& value) {
            static_assert(std::is_arithmetic_v<T>, "Only arithmetic types supported");
            const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
            for (size_t i = 0; i < sizeof(T); ++i) {
                bytecode.push_back(bytes[i]);
            }
    }
public:
    ByGen(std::vector<std::string> ir);
    std::vector<uint8_t>  generate();
};

#endif //BYGEN_H
