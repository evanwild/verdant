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

class Parser {
   public:
    Parser(std::vector<Token> tokens);

    ProgramNode make_tree();

   private:
    bool match(TokenKind kind);

    ComponentNode component();

    std::vector<Token> m_tokens;
    std::size_t m_index;
};

#endif  // PARSER_H
