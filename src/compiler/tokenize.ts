export enum TokenKind {
  LeftCurly = 'LeftCurly',
  RightCurly = 'RightCurly',
  LeftParen = 'LeftParen',
  RightParen = 'RightParen',
  Comp = 'Comp',
  Identifier = 'Identifier',
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
  ['comp', TokenKind.Comp],
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
      while (isAlphanumeric(peek())) {
        lexeme += consume();
      }
      const kind = lexemeToKind.get(lexeme) ?? TokenKind.Identifier;
      tokens.push({ kind, lexeme });
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
