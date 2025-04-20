cmake_minimum_required(VERSION 3.14)
project(kimi)

set(CMAKE_CXX_STANDARD 20)


add_library(kimi STATIC
        src/lexer/lexer.cpp
        src/lexer/lexer_tokens.cpp
        src/parser/parser.cpp
        src/parser/parser_expression.cpp
        src/ast/ast.cpp
        src/parser/include/parser_statement.h
        src/parser/parser_statement.cpp
        src/parser/include/parse_declaration.h
        src/parser/parse_declaration.cpp
        src/bygen/include/bygen.h
        src/bygen/include/bygen_expression.h
        src/bygen/bygen_expression.cpp
        src/bygen/bygen.cpp
        src/bygen/include/bygen_statement.h
        src/bygen/include/bygen_declaration.h
        src/bygen/bygem_declaration.cpp
)

add_subdirectory(tests)

target_include_directories(kimi PUBLIC
        src/lexer/include
        src/parser/include
        src/ast/include
        src/bygen/include
)
