#include "scanner.h"

#include <cctype>
#include <format>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

const std::unordered_map<std::string, TokenKind> keywords = {
    {"comp", TokenKind::Component},
};

std::vector<Token> scan_tokens(const std::string& source) {
    std::vector<Token> result;
    std::size_t index = 0;
    std::size_t line = 1;

    while (index < source.length()) {
        const char ch = source[index];
        ++index;

        if (ch == '\n') {
            ++line;
        } else if (std::isspace(ch)) {
            continue;
        } else if (ch == '(') {
            result.emplace_back(TokenKind::LParen, "(");
        } else if (ch == ')') {
            result.emplace_back(TokenKind::RParen, ")");
        } else if (ch == '{') {
            result.emplace_back(TokenKind::LCurly, "{");
        } else if (ch == '}') {
            result.emplace_back(TokenKind::RCurly, "}");
        } else if (std::isalpha(ch)) {
            TokenKind kind;
            std::string lexeme;

            lexeme += ch;
            while (index < source.length() && isalpha(source[index])) {
                lexeme += source[index];
                ++index;
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
