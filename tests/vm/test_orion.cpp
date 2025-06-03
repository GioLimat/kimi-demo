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
    auto code = "var x = -2;  var y = 2; ++y; y++; println(--y); if (x < y) { println(21 << 4); } ";
    runOrion(code);
}


TEST(Orion, SimpleCode2) {
    auto code = "var x = 2;  while (x < 10) { println(x++); } ";
    runOrion(code);
}


TEST(Orion, SimpleCode3) {
    auto code = "fn a(b:  Int, c: Int) {println(b); c;} val  k = a(10, 20); println(k + 4);";
    runOrion(code);
}


TEST(Orion, SimpleCode4) {
    auto code = "fn a(b:  Int) : Int { if ( b == 1){ return 1;} return b * a(b - 1); } println(a(5));";
    runOrion(code);
}


TEST(Orion, SimpleCode5) {
    auto code = "val b = 4; if (b == 3) { println(b); } else { println(12); } val c = 10; println(c); ";
    runOrion(code);
}


TEST(Orion, SimpleCode6) {
    auto code = "fn a() { val x = 1; println(x); fn b() { println(x); 4; } }  a();";
    runOrion(code);
}


TEST(Orion, SimpleCode7) {
    auto code = "fn a() {fn b() { println(4); 4;} b(); 4;} a(); ";
    runOrion(code);
}

TEST(Orion, SimpleCode8) {
    auto code = "fn fib (a: Int) : Int { if ( a  <= 1 ) { return a; } return fib(a - 1) + fib(a - 2); } println(fib(6)); ";
    runOrion(code);
}