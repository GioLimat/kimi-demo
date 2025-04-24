//
// Created by home on 22/04/25.
//

#include "ir_gen.h"
#include <iostream>
#include <ir_gen_expression.h>

#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>
#include "semantic_analyzer.h"

std::vector<std::string> gen(const std::string& code) {
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    SemanticAnalyzer semantic_analyzer;
    semantic_analyzer.analyze(*ast);
    return IRGen::generate(ast);
    // otimizer
    // bygen
}

TEST(IRGen, SimpleExpression) {
    const std::string code = "var x = true == false;";
    auto ir = gen(code);
    for (const auto& i : ir) {
        std::cout << i << std::endl;
    }
}