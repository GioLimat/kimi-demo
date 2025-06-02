//
// Created by home on 02/06/25.
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

void  runPlayground(const std::string& code) {
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    SemanticAnalyzer semantic_analyzer;
    semantic_analyzer.analyze(*ast);
    IRGen ir_gen(ast);
    auto ir = ir_gen.generate();
    auto by_gen = ByGen(ir);
    auto gen = by_gen.generate();
    std::cout << std::endl;
    OrionVM vm(gen);
    vm.preprocessFunctions();
    vm.run();
}



TEST(Code, Playground) {
    /*digite seu codigo aq*/
    auto code = " ";
    runPlayground(code);
}