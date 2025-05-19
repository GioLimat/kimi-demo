//
// Created by home on 22/04/25.
//

#include "ir_gen.h"
#include <iostream>

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
    IRGen ir_gen(ast);
    return ir_gen.generate();
}

TEST(IRGen, SimpleExpression) {
    const std::string code = "val x = 4 + 22222;";
    auto ir = gen(code);
    for (const auto& i : ir) {
        std::cout << i << std::endl;
    }
}