//
// Created by home on 03/06/25.
//

#include <CLI/CLI.hpp>
#include <iostream>
#include <string>
#include <exception>
#include <filesystem>
#include "reader.h++"
#include "pipeline.h++";

int main(int argc, char** argv) {
    CLI::App app{"Kimi CLI - read and process a source file"};

    std::string file_path;
    app.add_option("output", file_path, "Input source file")->required();

    CLI11_PARSE(app, argc, argv);

    try {
        const std::filesystem::path p(file_path);
        const std::string absolute_path = std::filesystem::absolute(p).string();
        const std::string contents = Reader::read(absolute_path);

        runVm(contents);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
