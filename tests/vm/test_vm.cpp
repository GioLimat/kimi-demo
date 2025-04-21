//
// Created by home on 20/04/25.
//

#include "bygen.h"
#include "bygen_expression.h"
#include "parser_expression.h"
#include "parser.h"
#include "lexer.h"
#include <gtest/gtest.h>

#include "bygen_declaration.h"
#include "parse_declaration.h"
#include "shin_VM.h"


void runVmStatement(const std::string& code) {
    Lexer lexer(code);
    const auto tokens = lexer.tokenize();
    ParserDeclaration parser(tokens);
    auto expr = parser.parseDeclaration();
    const auto bytecode = ByGenDeclaration::generateDeclaration(expr.get());
    auto vm = ShinVm(bytecode);
    vm.execute();
}


void runVM(const std::string& code) {
    Lexer lexer(code);
    const auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    const auto bytecode = ByGen::generate(ast);
    auto vm = ShinVm(bytecode);
    vm.execute();
}




TEST (ShinVmTest, SimpleExpression) {
    const std::string code = "var x = 2 + 4;";
    runVmStatement(code);
}


TEST (ShinVmTest, LoadVariable) {
    runVM("var x = 4 * (4 + 4 / 2) * 4 / 32 + 4 * 67;");
}
