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
        src/kimiIR/instructions/include/ir_instructions.h
        src/kimiIR/ir_gen.cpp
        src/kimiIR/ir_gen_declaration.cpp
        src/kimiIR/ir_gen_expression.cpp
        src/kimiIR/instructions/ir_instructions.cpp
        src/validator/semantic/include/semantic_analyzer.h
        src/validator/type_infer/include/type_analyzer.h
        src/validator/semantic/semantic_analyzer.cpp
        src/validator/type_infer/type_analyzer.cpp
        src/ast/include/visitor.h
        src/kimiIR/ir_gen_statement.cpp
        src/bygen/instructions/include/bygen_instructions.h
        src/bygen/instructions/bygen_instructions.cpp
        src/bygen/include/bygen.h
        src/bygen/bygen.cpp
        src/general/include/value.h
        src/general/include/sizes.h
        src/orion/include/orion_vm.h
        src/orion/orion_vm.cpp
        src/orion/print_handlers.h++
        src/orion/add_handlers.h++
        src/orion/sub_handlers.h++
        src/orion/mul_handlers.h++
        src/orion/div_handlers.h++
        src/orion/include/mod_handlers.h++
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
        src/bygen/instructions/include
        src/bygen/include
        src/general/include
        src/orion/include
)
