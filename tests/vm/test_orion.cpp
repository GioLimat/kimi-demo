//
// Created by home on 22/05/25.
//

#include <iomanip>
#include <string>

#include "bygen.h"
#include "ir_gen.h"
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include <gtest/gtest.h>
#include "pipeline.h++";

void  runOrion(const std::string& code) {
    /*Lexer lexer(code);
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
    auto by_gen = ByGen(ir, vm);
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
    vm.bytecode = gen;
    vm.preprocessFunctions();
    vm.run();*/
    runVm(code);
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
    auto code = "fn a(b:  Int) : Int { if ( b == 1 ){ return 1;} return b * a(b - 1); } println(a(5));";
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


TEST(Orion, SimpleCode9) {
    auto code = "var x = 1; do { println(x++); } while (x < 4) println(1090);";
    runOrion(code);
}

TEST(Orion, SimpleCode10) {
    auto code = "var i : Int; for (i = 0; i < 10; i++) { println(i); } println(i);";
    runOrion(code);
}

TEST(Orion, SimpleCode11) {
    auto code = "var x = 1; var b = 4; fn a(){ x = 4; return 4;} a(); if (x == 4) { x = 2;  }  println(b); b= 1; println(x);  println(b);";
    runOrion(code);
}

TEST(Orion, SimpleCode12) {
    auto code = "fn isEven(a: Int): Bool { a % 2 == 0 } println(isEven(4));  ";
    runOrion(code);
}


TEST(Orion, SimpleCode13) {
    auto code = "println(\"olá mundo isso é grande vai para o heap!\");";
    runOrion(code);
}

TEST(Orion, SimpleCode14) {
    auto code = "var x : Str = \" essa é a continuação da string \"; println(\"olá mundo isso é grande vai para o heap!\" + x); ";
    runOrion(code);
}
TEST(Orion, SimpleCode15) {
    auto code = "println([[4, 4] ,[4, 6, 7]]);" ;
    runOrion(code);
}


TEST(Orion, SimpleCode16) {
    auto code = "val x : Array[Array[Int]] = [[10,20], [30, 40]];  for (var i = 0; i < 2; i++) { for (var j = 0; j < 2; j++) { println(x[i][j]); } }" ;
    runOrion(code);
}

TEST(Orion, SimpleCode17) {
    auto code = "var arr = [10]; insert!(arr, 1, 40); insert!(arr, 0, 25); println(arr); remove!(arr, 1);  println(arr);" ;
    runOrion(code);
}

TEST(Orion, SimpleCode18) {
    auto code = "var arr : Array[Str] = [\"dsadasd\"]; insert!(arr, 1, \"dsadasd2\"); insert!(arr, 0,\"dsadasd3\"); println(arr); remove!(arr, 1);  println(arr);" ;
    runOrion(code);
}




TEST(Orion, SimpleCode19) {
    auto code = "println(3.2 as Int);" ;
    runOrion(code);
}
