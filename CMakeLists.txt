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
        src/bygen/bygen_expression.cpp
        src/bygen/bygen.cpp
        src/bygen/bygen_declaration.cpp
        src/shinVM/include/shin_VM.h
        src/shinVM/shin_VM.cpp
        src/shinVM/include/stack_manager.h
        src/shinVM/stack_manager.cpp
        src/shinVM/executor/include/executor.h
        src/bytecode_instructions/include/instructions.h
        src/bytecode_instructions/instructions.cpp
        src/shinVM/decoder.cpp
        src/shinVM/executor/executor.cpp
        src/bytecode_instructions/include/instructions_arithmetic.h
        src/bytecode_instructions/instructions_arithmetic.cpp
        src/bytecode_instructions/instructions_lloc.cpp
        src/bytecode_instructions/instructions_macros.cpp
        src/parser/parser_macro.cpp
        src/parser/include/parser_macro.h
        src/bygen/bygen_macros.cpp
        src/bytecode_instructions/include/instructions_macros.h
)

add_subdirectory(tests)

target_include_directories(kimi PUBLIC
        src/lexer/include
        src/parser/include
        src/ast/include
        src/bygen/include
        src/bytecode_instructions/include
        src/shinVM/include
        src/shinVM/executor/include
)
