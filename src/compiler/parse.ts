import { type Token, TokenKind } from './tokenize';

export enum NodeKind {
  Component = 'Component',
  StateDeclaration = 'StateDeclaration',
  NumberLiteral = 'NumberLiteral',
  HTML = 'HTML',
}

export type ComponentNode = {
  kind: NodeKind.Component;
  name: string;
  body: StatementNode[];
  html: HTMLNode[];
};

export type StateDeclarationNode = {
  kind: NodeKind.StateDeclaration;
  name: string;
  initialValue: ExpressionNode;
};

export type NumberLiteralNode = {
  kind: NodeKind.NumberLiteral;
  value: number;
};

export type HTMLNode = {
  kind: NodeKind.HTML;
  tagName: string;
};

export type ExpressionNode = NumberLiteralNode;

export type StatementNode = StateDeclarationNode;

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
    const body: StatementNode[] = [];
    while (!isAtEnd() && !peek(TokenKind.Open)) {
      body.push(statementNode());
    }
    const html: HTMLNode[] = [];
    html.push(htmlNode());
    while (!isAtEnd() && !peek(TokenKind.RightCurly)) {
      html.push(htmlNode());
    }
    consume(TokenKind.RightCurly);
    return { kind: NodeKind.Component, name, body, html };
  };

  const stateDeclarationNode = (): StateDeclarationNode => {
    consume(TokenKind.State);
    consume(TokenKind.Identifier);
    const name = previous().lexeme;
    consume(TokenKind.Equals);
    const initialValue = expressionNode();
    consume(TokenKind.Semicolon);
    return { kind: NodeKind.StateDeclaration, name, initialValue };
  };

  const numberLiteralNode = (): NumberLiteralNode => {
    consume(TokenKind.NumberLiteral);
    const value = Number(previous().lexeme);
    return { kind: NodeKind.NumberLiteral, value };
  };

  const htmlNode = (): HTMLNode => {
    consume(TokenKind.Open);
    consume(TokenKind.Identifier);
    const tagName = previous().lexeme;
    consume(TokenKind.Close);
    consume(TokenKind.OpenSlash);
    consume(TokenKind.Identifier);
    const closingTagName = previous().lexeme;
    if (closingTagName !== tagName) {
      throw new Error(
        `Closing tag </${closingTagName}> doesn't match opening tag <${tagName}>`
      );
    }
    consume(TokenKind.Close);
    return { kind: NodeKind.HTML, tagName };
  };

  const expressionNode = (): ExpressionNode => {
    if (peek(TokenKind.NumberLiteral)) {
      return numberLiteralNode();
    }
    throw new Error(
      `Expected start of an expression but next token is: ${tokens[i].lexeme}`
    );
  };

  const statementNode = (): StatementNode => {
    if (peek(TokenKind.State)) {
      return stateDeclarationNode();
    }
    throw new Error(
      `Expected start of a statement but next token is: ${tokens[i].lexeme}`
    );
  };

  const result: ComponentNode[] = [];
  while (!isAtEnd()) {
    result.push(componentNode());
  }
  return result;
}
