//
// Created by home on 23/04/25.
//

#include <gtest/gtest.h>
#include "type_analyzer.h"
#include "parser.h"
#include "lexer.h"


TEST(TypeInferTest, BasicTypes) {
    Lexer lexer("42 + 3.14");
    Parser parser(lexer.tokenize());
    auto ast = parser.parse();

    auto expression = dynamic_cast<ExpressionNode*>(ast->children.at(0).get());

    auto result = TypeInfer::analyzeExpression(expression);

    std::cout << result << std::endl;
}