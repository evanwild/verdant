#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "scanner.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    std::ifstream source_file(argv[1]);
    if (!source_file) {
        std::cerr << "Failed to read file " << argv[1] << '\n';
        return 1;
    }

    std::stringstream buffer;
    buffer << source_file.rdbuf();
    source_file.close();

    try {
        auto tokens = scan_tokens(buffer.str());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::ofstream output_file("index.html");
    if (!output_file) {
        std::cerr << "Failed to write file index.html\n";
        return 1;
    }

    output_file << "<!DOCTYPE html><title>Verdant App</title>\n";

    return 0;
}
