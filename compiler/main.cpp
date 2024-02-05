#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "lexer.h"
#include "parser.h"
#include "semantic.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    std::ifstream source_file{argv[1]};
    if (!source_file) {
        std::cerr << "Failed to read file " << argv[1] << '\n';
        return 1;
    }

    // Read the whole source file into a std::string, see
    // https://stackoverflow.com/a/2602258
    std::stringstream buffer;
    buffer << source_file.rdbuf();
    source_file.close();

    Lexer lexer{buffer.str()};
    std::vector<Token> tokens;

    try {
        tokens = lexer.scan_tokens();
    } catch (const std::exception& e) {
        std::cerr << "Lexing error: " << e.what() << '\n';
        return 1;
    }

    Parser parser{std::move(tokens)};
    ProgramNode tree;

    try {
        tree = parser.make_tree();
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << '\n';
        return 1;
    }

    try {
        semantic_analyze(tree);
    } catch (const std::exception& e) {
        std::cerr << "Semantic error: " << e.what() << '\n';
        return 1;
    }

    std::ofstream output_file{"index.html"};
    if (!output_file) {
        std::cerr << "Failed to write file index.html\n";
        return 1;
    }

    output_file << "<!DOCTYPE html>\n"
                   "<html lang=\"en\">\n"
                   "\n"
                   "<head>\n"
                   "    <title>Verdant App</title>\n"
                   "</head>\n"
                   "\n"
                   "<body>\n"
                   "    <div id=\"root\"></div>\n"
                   "    <script>\n"
                   "        const root = document.getElementById(\"root\");\n"
                   "    </script>\n"
                   "</body>\n"
                   "\n"
                   "</html>\n";

    return 0;
}
