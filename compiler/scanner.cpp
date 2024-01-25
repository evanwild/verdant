#include "scanner.h"

#include <cctype>
#include <cstddef>
#include <format>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Scanner::Scanner(std::string source)
    : m_source{std::move(source)}, m_index{0} {}

char Scanner::peek() {
    if (m_index < m_source.size()) {
        return m_source[m_index];
    }
    return '\0';
}

char Scanner::consume() {
    if (m_index < m_source.size()) {
        return m_source[m_index++];
    }
    return '\0';
}

const std::map<std::string, TokenKind> keywords = {
    {"comp", TokenKind::Component},
};

std::vector<Token> Scanner::scan_tokens() {
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

            if (auto it = keywords.find(lexeme); it != keywords.end()) {
                kind = it->second;
            } else {
                kind = TokenKind::Identifier;
            }

            result.emplace_back(kind, std::move(lexeme));
            continue;
        }

        if (ch == '<') {
            if (peek() == '/') {
                consume();
                result.emplace_back(TokenKind::OpenSlash, "</");
                --num_open;
            } else {
                result.emplace_back(TokenKind::Open, "<");
                ++num_open;
            }

            if (isspace(peek())) {
                throw std::runtime_error(
                    "Scanning error: No whitespace allowed after < or </");
            }
            continue;
        }

        auto scan_string = [&]() {
            if (num_open > 0) {
                std::string lexeme;
                while (m_index < m_source.size() && peek() != '<') {
                    lexeme += consume();
                }
                result.emplace_back(TokenKind::String, std::move(lexeme));
            }
        };

        if (ch == '>') {
            result.emplace_back(TokenKind::Close, ">");
            scan_string();
            continue;
        }

        if (ch == '/' && peek() == '>') {
            consume();
            result.emplace_back(TokenKind::SlashClose, "/>");
            --num_open;
            scan_string();
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
        } else {
            const auto error_msg =
                std::format("Scanning error: Did not expect \'{}\'", ch);
            throw std::runtime_error(error_msg);
        }
    }

    return result;
}
