#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "parser.h"
#include "scanner.h"
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

    std::stringstream buffer;
    buffer << source_file.rdbuf();
    source_file.close();

    try {
        Scanner scanner{buffer.str()};
        const auto tokens = scanner.scan_tokens();

        Parser parser{std::move(tokens)};
        const auto tree = parser.make_tree();

        semantic_analyze(tree);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
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
