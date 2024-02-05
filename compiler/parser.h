#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <variant>
#include <vector>

#include "lexer.h"

struct HTMLNode {
    std::string tag_type;
    std::vector<std::variant<std::string, HTMLNode>> contents;
};

struct ComponentNode {
    std::string name;
    std::vector<HTMLNode> htmls;
};

struct ProgramNode {
    std::vector<ComponentNode> components;
};

class Parser {
   public:
    Parser(std::vector<Token> tokens);

    ProgramNode make_tree();

   private:
    void match(TokenKind kind);
    bool peek(TokenKind kind);
    Token prev();

    HTMLNode html();
    ComponentNode component();

    std::vector<Token> m_tokens;
    std::size_t m_index;
};

#endif  // PARSER_H
