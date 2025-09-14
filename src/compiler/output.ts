import {
  NodeKind,
  type ComponentNode,
  type ExpressionNode,
  type HTMLNode,
  type TextNode,
} from './parse.ts';
import { assertNever, explicitWhitespace } from './utils.ts';

let nextElemNum = 0;

export function outputJS(components: ComponentNode[]): string {
  return (
    components.map((comp) => outputComponent(comp)).join('\n') +
    "\ndocument.getElementById('root').append(new Main().rootElem);"
  );
}

function outputComponent(comp: ComponentNode): string {
  const stateDeclarations = comp.statements
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map((s) => `    this.${s.name}=${outputExpression(s.initialValue)};`)
    .join('\n');

  nextElemNum = 0;
  const domElements = outputDOMElements(comp.html);

  const rootElement =
    '    this.rootElem=new DocumentFragment();\n' +
    `    this.rootElem.append(${domElements.toAppend});`;

  const stateChangeHandlers = comp.statements
    .filter((stmt) => stmt.kind === NodeKind.StateDeclaration)
    .map((decl) => `  ${decl.name}Changed() {}`)
    .join('\n');

  return `class ${comp.name} {
  constructor() {
${stateDeclarations}
${domElements.js}
${rootElement}
  }
${stateChangeHandlers}
}`;
}

function outputDOMElements(nodes: (HTMLNode | TextNode)[]): {
  js: string;
  toAppend: string[];
} {
  const result = { js: '', toAppend: [] as string[] };

  for (const node of nodes) {
    if (node.kind === NodeKind.Text) {
      result.toAppend.push(`'${explicitWhitespace(node.text)}'`);
      continue;
    }

    let elemName = `this.elem${nextElemNum++}`;

    result.js += `    ${elemName}=document.createElement('${node.tagName}');\n`;

    const { js, toAppend } = outputDOMElements(node.children);

    result.js += js;
    result.js += `    ${elemName}.append(${toAppend.join(',')});\n`;

    result.toAppend.push(elemName);
  }

  return result;
}

function outputExpression(expr: ExpressionNode): string {
  if (expr.kind === NodeKind.NumberLiteral) {
    return expr.value.toString();
  }
  assertNever(expr.kind);
}
