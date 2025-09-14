export function assertNever(value: never): never {
  throw new Error(`assertNever failed for value: ${value}`);
}

export function explicitWhitespace(s: string) {
  let result = '';
  for (const ch of s) {
    if (ch === '\n') {
      result += '\\n';
    } else if (ch === '\t') {
      result += '\\t';
    } else {
      result += ch;
    }
  }
  return result;
}
