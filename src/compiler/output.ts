import { NodeKind, type ComponentNode, type ExpressionNode } from './parse.ts';
import { assertNever } from './utils.ts';

export function outputJS(components: ComponentNode[]): string {
  return (
    components.map((comp) => outputComponent(comp)).join('\n') +
    "\ndocument.getElementById('root').append(new Main().rootElem);"
  );
}

function outputComponent(comp: ComponentNode): string {
  const stateDeclarations = comp.body
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map((s) => `    this.${s.name}=${outputExpression(s.initialValue)};`)
    .join('\n');

  const domElements = comp.html
    .map(
      (h, idx) => `    this.elem${idx}=document.createElement('${h.tagName}')`
    )
    .join('\n');

  const rootElement =
    '    this.rootElem=new DocumentFragment();\n' +
    `    this.rootElem.append(${comp.html
      .map((_, idx) => `this.elem${idx}`)
      .join(',')});`;

  const stateChangeHandlers = comp.body
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map((decl) => `  ${decl.name}Changed() {}`)
    .join('\n');

  return `class ${comp.name} {
  constructor() {
${stateDeclarations}
${domElements}
${rootElement}
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
