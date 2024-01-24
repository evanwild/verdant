#ifndef SCANNER_H
#define SCANNER_H

#include <cstddef>
#include <string>
#include <vector>

enum class TokenKind {
    Identifier,
    Component,
    LParen,
    RParen,
    LCurly,
    RCurly,

    // HTML Specific
    Open,
    OpenSlash,
    Close,
    SlashClose,
    String,
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
    char peek();
    char consume();

    std::string m_source;
    std::size_t m_index;
};

#endif  // SCANNER_H
