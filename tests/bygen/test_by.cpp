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
    OrionVM vm;
    ByGen by_gen = ByGen(ir, vm);
    return by_gen.generate();
}


TEST(Bygen, SimpleBytecode) {
    auto byte = genBy("if (4 > 0){println(4)}");
    for (const auto& b : byte) {
        std::cout  << "0x"
            << std::hex
            << static_cast<int>(b)
           << " ";
    }
    std::cout << std::endl;
}


TEST(Bygen, Char) {
    auto byte = genBy("'á';");

    for (const auto& b : byte) {
        std::cout << "0x"
            << std::hex
            << (int)b
           << " ";
    }
    std::cout << std::endl;
}

TEST(Bygen, StringLiteralSmalll) {
    auto byte = genBy("println(\"olá!\");");

    for (const auto& b : byte) {
        std::cout << "0x"
            << std::hex
            << (int)b
           << " ";
    }
    std::cout << std::endl;
}

TEST(Bygen, StringLiteralLarge) {
    auto byte = genBy("println(\"olá mundo isso é grande vai para o heap!\");");

    for (const auto& b : byte) {
        std::cout << "0x"
            << std::hex
            << (int)b
           << " ";
    }
    std::cout << std::endl;
}