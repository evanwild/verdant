#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "scanner.h"

struct ComponentNode {
    std::string name;
};

struct ProgramNode {
    std::vector<ComponentNode> components;
};

ProgramNode parse_tokens(const std::vector<Token> &tokens);

#endif  // PARSER_H
