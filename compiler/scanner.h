#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

enum class TokenKind {
    Component,
    Identifier,
    LParen,
    RParen,
    LCurly,
    RCurly,
};

struct Token {
    TokenKind kind;
    std::string lexeme;
};

std::vector<Token> scan_tokens(const std::string& source);

#endif  // SCANNER_H
