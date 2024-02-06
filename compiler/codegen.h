#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>

#include "parser.h"

std::string generate_code(const ProgramNode &tree);

#endif  // CODEGEN.H
