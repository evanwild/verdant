import { NodeKind, type ComponentNode, type ExpressionNode } from './parse.ts';
import { assertNever } from './utils.ts';

export function outputJS(components: ComponentNode[]): string {
  return components.map((comp) => outputComponent(comp)).join('\n');
}

function outputComponent(comp: ComponentNode): string {
  const stateDeclarations = comp.body
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map(
      (decl) => `    this.${decl.name}=${outputExpression(decl.initialValue)};`
    )
    .join('\n');

  const stateChangeHandlers = comp.body
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map((decl) => `  ${decl.name}Changed() {}`)
    .join('\n');

  return `class ${comp.name} {
  constructor() {
${stateDeclarations}
  }
${stateChangeHandlers}
}`;
}

function outputExpression(expr: ExpressionNode): string {
  if (expr.kind === NodeKind.NumberLiteral) {
    return expr.value.toString();
  }
  assertNever(expr.kind);
}
