#ifndef SCANNER_H
#define SCANNER_H

#include <cstddef>
#include <string>
#include <vector>

enum class TokenKind {
    // Single-character
    LParen,
    RParen,
    LCurly,
    RCurly,
    LAngle,
    RAngle,
    FSlash,

    // Literals
    Identifier,

    // Keywords
    Component,
};

struct Token {
    TokenKind kind;
    std::string lexeme;
};

class Scanner {
   public:
    Scanner(std::string source);

    std::vector<Token> scan_tokens();

   private:
    std::string m_source;
    std::size_t m_index;
};

#endif  // SCANNER_H
