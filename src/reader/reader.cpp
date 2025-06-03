//
// Created by home on 03/06/25.
//
#include "reader.h++"

#include <fstream>
#include <sstream>
#include <stdexcept>


std::string Reader::read(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}