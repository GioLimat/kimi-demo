//
// Created by home on 22/05/25.
//

#include <iomanip>
#include <string>

#include "bygen.h"
#include "ir_gen.h"
#include "lexer.h"
#include "orion_vm.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include <gtest/gtest.h>

void  runOrion(const std::string& code) {
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
    OrionVM vm(gen);
    vm.preprocessFunctions();
    vm.run();
}



TEST(Orion, SimpleCode) {
    auto code = "var x = -2;  var y = 2; ++y; y++; println(--y); if (x < y) { println(21); } ";
    runOrion(code);
}


TEST(Orion, SimpleCode2) {
    auto code = "var x = 2;  while (x < 10) { println(x++); } ";
    runOrion(code);
}


TEST(Orion, SimpleCode3) {
    auto code = "fn a() { 4;} a();";
    runOrion(code);
}