export enum TokenKind {
  LeftCurly = 'LeftCurly',
  RightCurly = 'RightCurly',
  LeftParen = 'LeftParen',
  RightParen = 'RightParen',
  Semicolon = 'Semicolon',
  Equals = 'Equals',
  Comp = 'Comp',
  State = 'State',
  Identifier = 'Identifier',
  NumberLiteral = 'NumberLiteral',
}

export type Token = {
  kind: TokenKind;
  lexeme: string;
};

const lexemeToKind = new Map([
  ['{', TokenKind.LeftCurly],
  ['}', TokenKind.RightCurly],
  ['(', TokenKind.LeftParen],
  [')', TokenKind.RightParen],
  [';', TokenKind.Semicolon],
  ['=', TokenKind.Equals],
  ['comp', TokenKind.Comp],
  ['state', TokenKind.State],
]);

export function tokenize(s: string): Token[] {
  const tokens: Token[] = [];

  let i = 0;
  const peek = () => s[i] ?? '\0';
  const consume = () => s[i++] ?? '\0';

  while (peek() !== '\0') {
    let lexeme = consume();

    if (isWhitespace(lexeme)) {
      continue;
    }

    if (isAlpha(lexeme)) {
      // Read in the rest of keyword or identifier
      while (isAlphanumeric(peek())) {
        lexeme += consume();
      }
      const kind = lexemeToKind.get(lexeme) ?? TokenKind.Identifier;
      tokens.push({ kind, lexeme });
      continue;
    }

    if (isDigit(lexeme)) {
      // Read in the rest of number literal
      while (isDigit(peek())) {
        lexeme += consume();
      }
      tokens.push({ kind: TokenKind.NumberLiteral, lexeme });
      continue;
    }

    const kind = lexemeToKind.get(lexeme);

    if (kind !== undefined) {
      tokens.push({ kind, lexeme });
      continue;
    }

    throw new Error(`Unexpected character ${lexeme} while tokenizing`);
  }

  return tokens;
}

function isWhitespace(ch: string): boolean {
  return ' \n\t\v\f\r'.includes(ch);
}

function isAlpha(ch: string): boolean {
  return 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'.includes(ch);
}

function isDigit(ch: string): boolean {
  return '0123456789'.includes(ch);
}

function isAlphanumeric(ch: string): boolean {
  return isAlpha(ch) || isDigit(ch);
}
