import './style.css';
import { tokenize } from './compiler/tokenize';
import { parse } from './compiler/parse';
import { outputJS } from './compiler/output';

const inputArea = document.getElementById(
  'input-textarea'
) as HTMLTextAreaElement;

const outputArea = document.getElementById(
  'output-textarea'
) as HTMLTextAreaElement;

// Load example program
inputArea.value = `comp Main() {
	state count = 0;

	<div>
		<h1>Count is 0</h1>
		<button>Increment</button>
		<button>Decrement</button>
	</div>
}
`;

function recompileSource() {
  try {
    const tokens = tokenize(inputArea.value);
    const components = parse(tokens);
    outputArea.value = outputJS(components);
  } catch (e) {
    outputArea.value = '❌ ' + e;
  }
}

recompileSource();
inputArea.addEventListener('input', recompileSource);

// Pressing tab should insert the tab character, not change the focused element
inputArea.addEventListener('keydown', (e) => {
  if (e.code === 'Tab') {
    e.preventDefault();

    const { selectionStart, selectionEnd } = inputArea;

    inputArea.value =
      inputArea.value.slice(0, selectionStart) +
      '\t' +
      inputArea.value.slice(selectionEnd);

    inputArea.selectionStart = inputArea.selectionEnd = selectionStart + 1;

    recompileSource();
  }
});
