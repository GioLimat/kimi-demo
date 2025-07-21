cmake_minimum_required(VERSION 3.14)
project(kimi)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Enable building of unit/integration tests" ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
endif()

if(WIN32)
    file(TO_CMAKE_PATH "$ENV{USERPROFILE}" USERPROFILE_PATH)
    set(USER_KIMI_DIR "${USERPROFILE_PATH}/Tools/kimi")
    file(MAKE_DIRECTORY ${USER_KIMI_DIR})
    message(STATUS "Will install Kimi to: ${USER_KIMI_DIR}")
    set(CMAKE_INSTALL_PREFIX "${USER_KIMI_DIR}")
endif()

include(FetchContent)
FetchContent_Declare(
        cli11_proj
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11_proj)

set(RUST_CRATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/rust)

add_custom_command(
        OUTPUT ${RUST_CRATE_DIR}/target/release/kimi.lib
        COMMAND cargo build --release
        WORKING_DIRECTORY ${RUST_CRATE_DIR}
        COMMENT "Building Rust crate in ${RUST_CRATE_DIR}"
)

add_custom_target(rust_build ALL
        DEPENDS ${RUST_CRATE_DIR}/target/release/kimi.lib
)

add_library(rust_lib STATIC IMPORTED GLOBAL)
set_target_properties(rust_lib PROPERTIES
        IMPORTED_LOCATION ${RUST_CRATE_DIR}/target/release/kimi.lib
)
add_library(kimi STATIC
        src/lexer/lexer.cpp
        src/lexer/lexer_tokens.cpp
        src/parser/parser.cpp
        src/parser/parser_expression.cpp
        src/ast/ast.cpp
        src/parser/parser_statement.cpp
        src/parser/parse_declaration.cpp
        src/kimiIR/ir_gen.cpp
        src/kimiIR/ir_gen_declaration.cpp
        src/kimiIR/ir_gen_expression.cpp
        src/kimiIR/instructions/ir_instructions.cpp
        src/validator/semantic/semantic_analyzer.cpp
        src/validator/type_infer/type_analyzer.cpp
        src/kimiIR/ir_gen_statement.cpp
        src/bygen/instructions/bygen_instructions.cpp
        src/bygen/bygen.cpp
        src/bygen/bygen_helpers.cpp
        src/reader/reader.cpp
        src/pipeline/pipeline.cpp
)

target_include_directories(kimi PUBLIC
        src/lexer/include
        src/parser/include
        src/ast/include
        src/kimiIR/include
        src/kimiIR/instructions/include
        src/validator/semantic/include
        src/validator/type_infer/include
        src/bygen/instructions/include
        src/bygen/include
        src/general/include
        src/primitives/include
        src/reader/include
        src/pipeline/include
)

add_executable(kimi_cli src/cli/cli.cpp)
set_target_properties(kimi_cli PROPERTIES OUTPUT_NAME kimi)
target_link_libraries(kimi_cli
        PRIVATE
        kimi
        ws2_32
        rust_lib
        CLI11::CLI11
        advapi32
        ntdll
        userenv
)

install(TARGETS kimi_cli
        RUNTIME DESTINATION bin
)

if(WIN32)
    add_custom_command(TARGET kimi_cli POST_BUILD
            COMMAND powershell -ExecutionPolicy Bypass -File ${CMAKE_SOURCE_DIR}/path.ps1
            COMMENT "Running PowerShell to update user's PATH"
    )
endif()

#if(BUILD_TESTS)
add_subdirectory(tests)
#endif()