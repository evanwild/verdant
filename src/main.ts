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
	// No need for setters! The compiler knows where each state variable changes
	// and automatically inserts re-render logic
	state count = 0;

	// Implicit return and merging adjacent elements into fragments
	// saves two indents vs the required React code!
	// return (
	// \t<>
	// \t\t<h1></h1>
	// \t\t...
	// \t</>
	// );

	<h1></h1>
	<button></button>
	<button></button>
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
