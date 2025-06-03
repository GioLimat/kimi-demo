enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

# Só adiciona o subdiretório do googletest se os alvos ainda não existirem
if(NOT TARGET GTest::gtest)
    add_subdirectory(
            ${googletest_SOURCE_DIR}
            ${googletest_BINARY_DIR}
    )
endif()

add_executable(kimi_tests
        lexer/test_lexer.cpp
        parser/test_parser_expression.cpp
        parser/test_parser_declaration.cpp
        parser/test_parser.cpp
        kimiIR/test_ir.cpp
        type_infer/test_type_infer.cpp
        bygen/test_by.cpp
        vm/test_orion.cpp
        playground/playground.cpp
)

target_link_libraries(kimi_tests
        PRIVATE
        kimi
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(kimi_tests)
