#include "scanner.h"

#include <cctype>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

const std::unordered_map<std::string, TokenKind> keywords = {
    {"comp", TokenKind::Component},
};

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

std::vector<Token> Scanner::scan_tokens() {
    std::vector<Token> result;
    std::size_t line = 1;  // Used for error messages

    while (m_index < m_source.length()) {
        const char ch = consume();

        // HTML rule: no whitespace between "<" or "</" and tag name
        if (!result.empty() && std::isspace(ch)) {
            if (result.back().kind == TokenKind::Open ||
                result.back().kind == TokenKind::OpenSlash) {
                const auto error_msg = std::format(
                    "Scanning error: No whitespace allowed after \"{}\" on "
                    "line {}",
                    result.back().lexeme, line);
                throw std::runtime_error(error_msg);
            }
        }

        if (ch == '\n') {
            ++line;
        } else if (std::isspace(ch)) {
            continue;
        } else if (ch == '<' && peek() == '/') {
            consume();
            result.emplace_back(TokenKind::OpenSlash, "</");
        } else if (ch == '/' && peek() == '>') {
            consume();
            result.emplace_back(TokenKind::SlashClose, "/>");
        } else if (ch == '(') {
            result.emplace_back(TokenKind::LParen, "(");
        } else if (ch == ')') {
            result.emplace_back(TokenKind::RParen, ")");
        } else if (ch == '{') {
            result.emplace_back(TokenKind::LCurly, "{");
        } else if (ch == '}') {
            result.emplace_back(TokenKind::RCurly, "}");
        } else if (ch == '<') {
            result.emplace_back(TokenKind::Open, "<");
        } else if (ch == '>') {
            result.emplace_back(TokenKind::Close, ">");
        } else if (std::isalpha(ch)) {
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
        } else {
            const auto error_msg = std::format(
                "Scanning error: Did not expect \'{}\' on line {}", ch, line);
            throw std::runtime_error(error_msg);
        }
    }

    return result;
}
