import { type ComponentNode } from './parse.ts';

export function outputJS(components: ComponentNode[]): string {
  let s = '';

  // Specific Packet Types
  for (const comp of components) {
    s += `class ${comp.name} {}\n`;
  }

  return s;
}
