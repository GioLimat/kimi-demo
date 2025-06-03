cmake_minimum_required(VERSION 3.14)
project(kimi)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        cli11_proj
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11_proj)


message(STATUS "CLI11 source dir: ${cli11_proj_SOURCE_DIR}")

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
        src/orion/binaryOp/add_handlers.h++
        src/orion/binaryOp/sub_handlers.h++
        src/orion/binaryOp/mul_handlers.h++
        src/orion/binaryOp/div_handlers.h++
        src/orion/binaryOp/mod_handlers.h++
        src/orion/binaryOp/greater_handlers.h++
        src/orion/binaryOp/less_handlers.h++
        src/orion/binaryOp/equal_handlers.h++
        src/orion/binaryOp/ge_handlers.h++
        src/orion/binaryOp/le_handlers.h++
        src/orion/binaryOp/ne_handlers.h++
        src/orion/unary/not_handlers.h++
        src/orion/unary/neg_handlers.h++
        src/orion/logical/and_handlers.h++
        src/orion/logical/or_handlers.h++
        src/orion/binaryOp/bit_and_handlers.h++
        src/orion/binaryOp/bit_or_handlers.h++
        src/orion/binaryOp/bit_xor_handlers.h++
        src/orion/binaryOp/bit_sl_handlers.h++
        src/orion/binaryOp/bit_sr_handlers.h++
        src/orion/unary/bit_not_handlers.h++
        src/orion/preprocess_functions.cpp
        src/bygen/bygen_helpers.cpp
        src/reader/reader.cpp
)
add_executable(kimi_cli src/cli/cli.cpp)
target_link_libraries(kimi_cli PRIVATE kimi CLI11::CLI11)

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
        src/orion/binaryOp
        src/orion/unary
        src/reader/include
)
