import { type Token, TokenKind } from './tokenize';

export enum NodeKind {
  Component = 'Component',
}

export type ComponentNode = {
  kind: NodeKind.Component;
  name: string;
};

export function parse(tokens: Token[]): ComponentNode[] {
  let i = 0;

  const isAtEnd = () => !(i < tokens.length);

  const previous = () => tokens[i - 1];

  const peek = (kind: TokenKind) => {
    if (isAtEnd()) return false;
    return tokens[i].kind === kind;
  };

  const tryConsume = (kind: TokenKind) => {
    if (peek(kind)) {
      i++;
      return true;
    }
    return false;
  };

  const consume = (kind: TokenKind) => {
    if (!tryConsume(kind)) {
      throw new Error(`Expected ${kind} token while parsing`);
    }
  };

  const componentNode = (): ComponentNode => {
    consume(TokenKind.Comp);
    consume(TokenKind.Identifier);
    const name = previous().lexeme;
    consume(TokenKind.LeftParen);
    consume(TokenKind.RightParen);
    consume(TokenKind.LeftCurly);
    consume(TokenKind.RightCurly);
    return { kind: NodeKind.Component, name };
  };

  const result: ComponentNode[] = [];
  while (!isAtEnd()) {
    result.push(componentNode());
  }
  return result;
}
