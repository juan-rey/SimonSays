// make-pot.mjs — extracts gettext msgids from the plugin's PHP sources into
// languages/simonsays-board-editor.pot. Dependency-free; run with `node`.
// Accepts __( '...', $d ) and __( '...', 'simonsays-board-editor' ) plus the
// _e / esc_html__ / esc_html_e / esc_attr__ / esc_attr_e variants
// (single-quoted strings only, which is the plugin's convention).
import { readFileSync, writeFileSync, readdirSync, statSync } from 'node:fs';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = join(dirname(fileURLToPath(import.meta.url)), '..');

function phpFiles(dir) {
  const out = [];
  for (const name of readdirSync(dir)) {
    const p = join(dir, name);
    const st = statSync(p);
    if (st.isDirectory()) {
      if (!['node_modules', 'languages', 'tools', 'tests'].includes(name)) out.push(...phpFiles(p));
    } else if (name.endsWith('.php')) {
      out.push(p);
    }
  }
  return out;
}

const RX = /(?:__|_e|esc_html__|esc_html_e|esc_attr__|esc_attr_e)\(\s*'((?:[^'\\]|\\.)*)'\s*,\s*(?:\$d|'simonsays-board-editor')\s*\)/g;

const msgids = new Set();
for (const file of phpFiles(root)) {
  const src = readFileSync(file, 'utf8');
  for (const m of src.matchAll(RX)) {
    const raw = m[1].replace(/\\'/g, "'").replace(/\\\\/g, '\\');
    msgids.add(raw);
  }
}

const esc = (s) => s.replace(/\\/g, '\\\\').replace(/"/g, '\\"').replace(/\n/g, '\\n');
const sorted = [...msgids].sort((a, b) => a.localeCompare(b));

let pot = `# SimonSays Board Editor — translation template.
msgid ""
msgstr ""
"Project-Id-Version: SimonSays Board Editor 0.1.0\\n"
"MIME-Version: 1.0\\n"
"Content-Type: text/plain; charset=UTF-8\\n"
"Content-Transfer-Encoding: 8bit\\n"
"X-Generator: tools/make-pot.mjs\\n"

`;
for (const id of sorted) {
  pot += `msgid "${esc(id)}"\nmsgstr ""\n\n`;
}

writeFileSync(join(root, 'languages', 'simonsays-board-editor.pot'), pot, 'utf8');
console.log(`Extracted ${sorted.length} unique strings.`);
for (const id of sorted) console.log('  ' + id);
