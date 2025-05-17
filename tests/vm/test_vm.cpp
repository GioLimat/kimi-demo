//
// Created by home on 03/05/25.
//
#include <iomanip>

#include "ir_gen.h"
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>

#include "bygen.h"
#include "semantic_analyzer.h"
#include "vm.h"

void  runVm(const std::string& code) {
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
    auto by_gen = ByGen(ir);
    auto gen = by_gen.generate();
    for (auto b : gen) {
        auto v = static_cast<unsigned int>(b);
        std::cout
            << "0x"
            << std::uppercase
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << v
            << std::dec
           << " ";
    }
    std::cout << std::endl;
    VM vm(gen);
    vm.preprocessFunctions();
    vm.run();
}


TEST(Bygen, SimpleCode) {
    runVm("var x : Int[32] = 1; var b = x; println(x + b);");
}