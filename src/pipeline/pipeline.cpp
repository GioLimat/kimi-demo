//
// Created by home on 05/07/25.
//


#include "lexer.h";
#include "parser.h";
#include "semantic_analyzer.h";
#include "ir_gen.h";
#include "bygen.h";


extern "C" void run_vm(const uint8_t* data, size_t len);

void  runVm(const std::string& code) {
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    SemanticAnalyzer semantic_analyzer;
    semantic_analyzer.analyze(*ast);
    IRGen ir_gen(ast);
    auto ir = ir_gen.generate();
    OrionVM vm;
    auto by_gen = ByGen(ir, vm);
    auto gen = by_gen.generate();
    run_vm(gen.data(), gen.size());
}