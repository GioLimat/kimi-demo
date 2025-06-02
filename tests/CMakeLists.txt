cmake_minimum_required(VERSION 3.14)
project(KimiTests)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

add_executable(kimi_tests lexer/test_lexer.cpp
        parser/test_parser_expression.cpp
        parser/test_parser_declaration.cpp
        parser/test_parser.cpp
        kimiIR/test_ir.cpp
        type_infer/test_type_infer.cpp
        bygen/test_by.cpp
        vm/test_orion.cpp
        playground/playground.cpp
)

target_link_libraries(kimi_tests GTest::gtest_main kimi gtest gtest_main)

include(GoogleTest)

add_test(NAME KimiTests COMMAND kimi_tests)
gtest_discover_tests(kimi_tests)


