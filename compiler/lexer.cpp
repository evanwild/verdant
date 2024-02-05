#include "lexer.h"

#include <cctype>
#include <cstddef>
#include <format>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Lexer::Lexer(std::string source) : m_source{std::move(source)}, m_index{0} {}

char Lexer::peek() {
    if (m_index < m_source.size()) {
        return m_source[m_index];
    }
    return '\0';
}

char Lexer::consume() {
    if (m_index < m_source.size()) {
        return m_source[m_index++];
    }
    return '\0';
}

const std::map<std::string, TokenKind> keywords = {
    {"comp", TokenKind::Component},
};

std::vector<Token> Lexer::scan_tokens() {
    std::vector<Token> result;
    int num_open = 0;

    while (m_index < m_source.size()) {
        const char ch = consume();

        if (std::isspace(ch)) {
            continue;
        }

        if (std::isalpha(ch)) {
            TokenKind kind;
            std::string lexeme;

            lexeme += ch;
            while (isalnum(peek())) {
                lexeme += consume();
            }

            if (keywords.contains(lexeme)) {
                kind = keywords.at(lexeme);
            } else {
                kind = TokenKind::Identifier;
            }

            result.emplace_back(kind, std::move(lexeme));
            continue;
        }

        if (ch == '(') {
            result.emplace_back(TokenKind::LParen, "(");
        } else if (ch == ')') {
            result.emplace_back(TokenKind::RParen, ")");
        } else if (ch == '{') {
            result.emplace_back(TokenKind::LCurly, "{");
        } else if (ch == '}') {
            result.emplace_back(TokenKind::RCurly, "}");
        } else if (ch == '<' && peek() == '/') {
            consume();
            result.emplace_back(TokenKind::OpenSlash, "</");
        } else if (ch == '<') {
            result.emplace_back(TokenKind::Open, "<");
        } else if (ch == '>') {
            result.emplace_back(TokenKind::Close, ">");
        } else if (ch == '/' && peek() == '>') {
            consume();
            result.emplace_back(TokenKind::SlashClose, "/>");
        } else {
            const auto error_msg = std::format("Did not expect \'{}\'", ch);
            throw std::runtime_error(error_msg);
        }

        // =========== Special rule handlers after pushing a token ===========

        if (result.back().kind == TokenKind::Open) {
            ++num_open;

            if (isspace(peek())) {
                throw std::runtime_error("No whitespace allowed after <");
            }

            continue;
        }

        if (result.back().kind == TokenKind::OpenSlash) {
            --num_open;

            if (isspace(peek())) {
                throw std::runtime_error("No whitespace allowed after </");
            }

            continue;
        }

        auto scan_string = [&]() {
            std::string lexeme;
            while (m_index < m_source.size() && peek() != '<') {
                lexeme += consume();
            }
            result.emplace_back(TokenKind::String, std::move(lexeme));
        };

        if (num_open > 0) {
            if (result.back().kind == TokenKind::Close) {
                scan_string();
                continue;
            }

            if (result.back().kind == TokenKind::SlashClose) {
                --num_open;
                scan_string();
                continue;
            }
        }
    }

    return result;
}
