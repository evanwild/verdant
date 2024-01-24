#include "parser.h"

#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "scanner.h"

// program      ->  component*
// component    ->  COMP ID LPAREN RPAREN LCURLY html* RCURLY
// html         ->  OPEN ID SLASHCLOSE
// html         ->  OPEN ID CLOSE (STRING | html)* OPENSLASH ID CLOSE

Parser::Parser(std::vector<Token> tokens)
    : m_tokens{std::move(tokens)}, m_index{0} {}

void Parser::match(TokenKind kind) {
    if (m_index >= m_tokens.size()) {
        throw std::runtime_error(
            "Parsing error: Reached end of file while parsing");
    }

    if (m_tokens[m_index].kind != kind) {
        const auto error_msg = std::format(
            "Parsing error: Did not expect \"{}\"", m_tokens[m_index].lexeme);
        throw std::runtime_error(error_msg);
    }

    ++m_index;
}

bool Parser::peek(TokenKind kind) {
    return m_index < m_tokens.size() && m_tokens[m_index].kind == kind;
}

Token Parser::prev() {
    if (m_index == 0) {
        throw std::out_of_range("Tried to check token at -1");
    }
    return m_tokens[m_index - 1];
}

HTMLNode Parser::html() {
    HTMLNode result;

    match(TokenKind::Open);
    match(TokenKind::Identifier);

    result.tag_type = prev().lexeme;

    match(TokenKind::Close);

    // TODO...

    match(TokenKind::OpenSlash);
    match(TokenKind::Identifier);

    if (result.tag_type != prev().lexeme) {
        const auto error_msg = std::format(
            "Parsing error: Expected closing tag for \"{}\" but found \"{}\"",
            result.tag_type, prev().lexeme);
        throw std::runtime_error(error_msg);
    }

    match(TokenKind::Close);

    return result;
}

ComponentNode Parser::component() {
    ComponentNode result;

    match(TokenKind::Component);
    match(TokenKind::Identifier);

    result.name = prev().lexeme;

    match(TokenKind::LParen);
    match(TokenKind::RParen);
    match(TokenKind::LCurly);

    while (peek(TokenKind::Open)) {
        result.htmls.push_back(html());
    }

    match(TokenKind::RCurly);

    return result;
}

ProgramNode Parser::make_tree() {
    ProgramNode result;

    while (m_index < m_tokens.size()) {
        result.components.push_back(component());
    }

    return result;
}
