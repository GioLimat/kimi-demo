//
// Created by home on 23/04/25.
//

#include <gtest/gtest.h>
#include "type_analyzer.h"
#include "parser.h"
#include "lexer.h"


TEST(TypeInferTest, BasicTypes) {
    Lexer lexer("do { var a = 1; } while (a < 10)");
    Parser parser(lexer.tokenize());
    auto ast = parser.parse();
    //auto expression = dynamic_cast<ExpressionNode*>(ast->children.at(0).get());

    SemanticAnalyzer semanticAnalyzer;
    semanticAnalyzer.analyze(*ast);

}