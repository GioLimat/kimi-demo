//
// Created by home on 22/04/25.
//

#include "ir_gen.h"
#include <iostream>
#include <ir_gen_expression.h>

#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>

std::vector<std::string> gen(const std::string& code) {
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    return IRGen::generate(ast);
}

TEST(IRGen, SimpleExpression) {
    std::string code = "2 + 3";
    auto ir = gen(code);
    for (const auto& i : ir) {
        std::cout << i << std::endl;
    }
}