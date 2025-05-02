//
// Created by home on 02/05/25.
//

#include <iomanip>

#include "ir_gen.h"
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>

#include "bygen.h"
#include "semantic_analyzer.h"

std::vector<uint8_t>  genBy(const std::string& code) {
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    SemanticAnalyzer semantic_analyzer;
    semantic_analyzer.analyze(*ast);
    IRGen ir_gen(ast);
    auto ir = ir_gen.generate();
    for (auto& instruction : ir) {
        std::cout << instruction << std::endl;
    }
    ByGen by_gen = ByGen(ir);
    return by_gen.generate();
}


TEST(Bygen, SimpleBytecode) {
    auto byte = genBy("var a = 1;");
    for (auto b : byte) {
        auto v = static_cast<unsigned int>(b);
        std::cout
            << "0x"
            << std::uppercase
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << v
            << std::dec
            << std::endl;
    }
}