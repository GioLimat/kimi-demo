cmake_minimum_required(VERSION 3.14)
project(kimi)

set(CMAKE_CXX_STANDARD 20)


add_library(kimi STATIC
        src/lexer/lexer.cpp
        src/lexer/lexer_tokens.cpp
        src/parser/parser.cpp
        src/parser/parser_expression.cpp
        src/ast/ast.cpp
        src/parser/parser_statement.cpp
        src/parser/parse_declaration.cpp
        src/parser/parser_macro.cpp
        src/parser/include/parser_macro.h
        src/kimiIR/instructions/include/ir_instructions.h
        src/kimiIR/ir_gen.cpp
        src/kimiIR/ir_gen_declaration.cpp
        src/kimiIR/ir_gen_expression.cpp
        src/kimiIR/instructions/ir_instructions.cpp
        src/validator/semantic/include/semantic_analyzer.h
        src/validator/type_infer/include/type_analyzer.h
        src/validator/semantic/semantic_analyzer.cpp
        src/validator/type_infer/type_analyzer.cpp
)

add_subdirectory(tests)

target_include_directories(kimi PUBLIC
        src/lexer/include
        src/parser/include
        src/ast/include
        src/kimiIR/include
        src/kimiIR/instructions/include
        src/kimiIR/optimizer/include
        src/validator/semantic/include
        src/validator/type_infer/include
)
