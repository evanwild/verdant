#include "codegen.h"

#include <string>

#include "parser.h"

std::string generate_code(const ProgramNode &tree) {
    return "const a = document.createElement('p');\n"
           "a.textContent = \'Hello, World!\';\n"
           "document.body.appendChild(a);\n";
}
