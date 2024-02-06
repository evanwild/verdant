#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "codegen.h"
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

    std::filesystem::path output_dir{"./dist"};

    std::filesystem::remove_all(output_dir);
    std::filesystem::create_directory(output_dir);

    std::ofstream index_html{output_dir / "index.html"};
    if (!index_html) {
        std::cerr << "Failed to create index.html\n";
        return 1;
    }

    std::ofstream script_js{output_dir / "script.js"};
    if (!script_js) {
        std::cerr << "Failed to create script.js\n";
        return 1;
    }

    index_html << "<!DOCTYPE html>\n"
                  "<html lang=\"en\">\n"
                  "\n"
                  "<head>\n"
                  "    <title>Verdant App</title>\n"
                  "    <script defer src=\"./script.js\"></script>\n"
                  "</head>\n"
                  "\n"
                  "<body>\n"
                  "</body>\n"
                  "\n"
                  "</html>\n";

    script_js << generate_code(tree);

    return 0;
}
