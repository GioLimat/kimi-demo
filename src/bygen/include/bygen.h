//
// Created by home on 02/05/25.
//

#ifndef BYGEN_H
#define BYGEN_H
#include <stack>
#include <string>
#include <vector>

class ByGen {
    std::stack<std::unordered_map<std::string, uint32_t>> symbolTable;
    size_t nextId = 0;
    std::vector<uint8_t> bytecode;
    std::vector<std::string> ir;
    [[nodiscard]] std::vector<std::string> splitBySpace(const std::string& str) const;
    uint32_t getIdentifierId(const std::string& name);
    void declareIdentifier(const std::string& name, bool isParam);

    template<typename T>
    void emitBasedOnType(const std::string& type, T value) {
        if (type == "i8") emit<int8_t>(value);
        if (type == "i32") emit<int32_t>(value);
        if (type == "f64") emit<double>(value);
        if (type == "bool") emit<uint8_t>(value);
    };

    void emitBasedOnType(const std::string& type);


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
