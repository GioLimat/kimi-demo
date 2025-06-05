//
// Created by home on 03/06/25.
//

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <exception>
#include <filesystem>
#include "reader.h++"
#include "bygen.h"
#include "ir_gen.h"
#include "lexer.h"
#include "orion_vm.h"
#include "parser.h"
#include "semantic_analyzer.h"

int main(int argc, char** argv) {
    CLI::App app{"Kimi CLI - read and process a source file"};

    std::string file_path;
    app.add_option("-f", file_path, "Input source file")->required();

    CLI11_PARSE(app, argc, argv);

    try {
        const std::filesystem::path p(file_path);
        const std::string absolute_path = std::filesystem::absolute(p).string();
        const std::string contents = Reader::read(absolute_path);

        Lexer lexer(contents);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto ast = parser.parse();
        SemanticAnalyzer semantic_analyzer;
        semantic_analyzer.analyze(*ast);
        IRGen ir_gen(ast);
        auto ir = ir_gen.generate();
        OrionVM vm;
        auto by_gen = ByGen(ir, vm);
        auto gen = by_gen.generate();
        std::cout << std::endl;
        vm.bytecode = gen;
        vm.preprocessFunctions();
        vm.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
