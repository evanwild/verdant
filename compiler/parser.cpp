#include "parser.h"

#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#include "scanner.h"

// program      ->  component*
// component    ->  COMP ID LPAREN RPAREN LCURLY RCURLY

ProgramNode parse_tokens(const std::vector<Token> &tokens) {
    std::size_t index = 0;

    auto match = [&](TokenKind kind) {
        if (index < tokens.size() && tokens[index].kind == kind) {
            ++index;
            return true;
        }
        return false;
    };

    auto component = [&]() {
        ComponentNode result;
        bool success = false;

        if (match(TokenKind::Component) && match(TokenKind::Identifier)) {
            result.name = tokens[index - 1].lexeme;

            if (match(TokenKind::LParen) && match(TokenKind::RParen) &&
                match(TokenKind::LCurly) && match(TokenKind::RCurly)) {
                success = true;
            }
        }

        if (!success) {
            const auto error_msg = std::format(
                "Parsing error: Tried to read a component but found \"{}\"",
                tokens[index].lexeme);
            throw std::runtime_error(error_msg);
        }

        return result;
    };

    ProgramNode result;
    while (index < tokens.size()) {
        result.components.push_back(component());
    }
    return result;
}
