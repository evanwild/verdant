#include "parser.h"

#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "scanner.h"

// program      ->  component*
// component    ->  COMP ID LPAREN RPAREN LCURLY RCURLY

Parser::Parser(std::vector<Token> tokens)
    : m_tokens{std::move(tokens)}, m_index{0} {}

bool Parser::match(TokenKind kind) {
    if (m_index < m_tokens.size() && m_tokens[m_index].kind == kind) {
        ++m_index;
        return true;
    }
    return false;
}

ComponentNode Parser::component() {
    ComponentNode result;
    bool success = false;

    if (match(TokenKind::Component) && match(TokenKind::Identifier)) {
        result.name = m_tokens[m_index - 1].lexeme;

        if (match(TokenKind::LParen) && match(TokenKind::RParen) &&
            match(TokenKind::LCurly) && match(TokenKind::RCurly)) {
            success = true;
        }
    }

    if (!success) {
        const auto error_msg = std::format(
            "Parsing error: Tried to parse component but found \"{}\"",
            m_tokens[m_index].lexeme);
        throw std::runtime_error(error_msg);
    }

    return result;
}

ProgramNode Parser::make_tree() {
    ProgramNode result;

    while (m_index < m_tokens.size()) {
        result.components.push_back(component());
    }

    return result;
}
