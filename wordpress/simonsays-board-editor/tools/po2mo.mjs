// po2mo.mjs — compiles every languages/*.po into its binary .mo counterpart.
// Dependency-free implementation of the (little-endian) MO format.
import { readFileSync, writeFileSync, readdirSync } from 'node:fs';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';

const langDir = join(dirname(fileURLToPath(import.meta.url)), '..', 'languages');

function parsePo(text) {
  const entries = new Map(); // msgid -> msgstr
  let msgid = null, msgstr = null, mode = '';
  const unquote = (line) => JSON.parse(line.trim()); // PO strings are JSON-compatible

  const flush = () => {
    if (msgid !== null && msgstr !== null && msgstr !== '') entries.set(msgid, msgstr);
    msgid = msgstr = null;
  };

  for (const raw of text.split(/\r?\n/)) {
    const line = raw.trim();
    if (line === '' || line.startsWith('#')) continue;
    if (line.startsWith('msgid ')) {
      flush();
      msgid = unquote(line.slice(6));
      mode = 'id';
    } else if (line.startsWith('msgstr ')) {
      msgstr = unquote(line.slice(7));
      mode = 'str';
    } else if (line.startsWith('"')) {
      if (mode === 'id') msgid += unquote(line);
      else if (mode === 'str') msgstr += unquote(line);
    }
  }
  flush();
  // The header entry (msgid "") must be present in the .mo as well.
  if (!entries.has('')) entries.set('', 'Content-Type: text/plain; charset=UTF-8\n');
  return entries;
}

function buildMo(entries) {
  const enc = new TextEncoder();
  const items = [...entries.entries()]
    .map(([id, str]) => ({ id: enc.encode(id), str: enc.encode(str) }))
    .sort((a, b) => {
      const n = Math.min(a.id.length, b.id.length);
      for (let i = 0; i < n; i++) if (a.id[i] !== b.id[i]) return a.id[i] - b.id[i];
      return a.id.length - b.id.length;
    });

  const n = items.length;
  const headerSize = 28;
  const tableSize = n * 8;
  let offset = headerSize + tableSize * 2;
  const chunks = [];
  const origTable = [];
  const transTable = [];
  for (const it of items) {
    origTable.push([it.id.length, offset]);
    chunks.push(it.id, Uint8Array.of(0));
    offset += it.id.length + 1;
  }
  for (const it of items) {
    transTable.push([it.str.length, offset]);
    chunks.push(it.str, Uint8Array.of(0));
    offset += it.str.length + 1;
  }

  const out = new Uint8Array(offset);
  const dv = new DataView(out.buffer);
  dv.setUint32(0, 0x950412de, true);  // magic
  dv.setUint32(4, 0, true);           // revision
  dv.setUint32(8, n, true);           // count
  dv.setUint32(12, headerSize, true); // offset of originals table
  dv.setUint32(16, headerSize + tableSize, true); // offset of translations table
  dv.setUint32(20, 0, true);          // hash size
  dv.setUint32(24, 0, true);          // hash offset
  origTable.forEach(([len, ofs], i) => {
    dv.setUint32(headerSize + i * 8, len, true);
    dv.setUint32(headerSize + i * 8 + 4, ofs, true);
  });
  transTable.forEach(([len, ofs], i) => {
    dv.setUint32(headerSize + tableSize + i * 8, len, true);
    dv.setUint32(headerSize + tableSize + i * 8 + 4, ofs, true);
  });
  let p = headerSize + tableSize * 2;
  for (const c of chunks) {
    out.set(c, p);
    p += c.length;
  }
  return out;
}

for (const name of readdirSync(langDir)) {
  if (!name.endsWith('.po')) continue;
  const entries = parsePo(readFileSync(join(langDir, name), 'utf8'));
  const mo = buildMo(entries);
  const outName = name.replace(/\.po$/, '.mo');
  writeFileSync(join(langDir, outName), mo);
  console.log(`${name} -> ${outName} (${entries.size} entries)`);
}
