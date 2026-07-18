// make-sample.mjs — builds a sample .ssz with the plugin's own modules
// (SPC template + one PNG icon + one MP3), for cross-checking against the
// SimonSays desktop importer. Usage: node tools/make-sample.mjs <out.ssz>
import { readFileSync, writeFileSync } from 'node:fs';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { parseSsc, serializeSsc, referencedResources } from '../assets/js/ssc.js';
import { buildSsz } from '../assets/js/ssz.js';

const root = join(dirname(fileURLToPath(import.meta.url)), '..');
const out = process.argv[2];
if (!out) {
  console.error('usage: node tools/make-sample.mjs <out.ssz>');
  process.exit(2);
}

const board = parseSsc(readFileSync(join(root, 'templates', 'spc-adult.ssc'), 'utf8'));

// Give the first category a PNG icon and its first phrase an MP3 sound.
board.categories[0].icon = 'icon1.png';
board.categories[0].phrases[0].audioFile = 'tada.mp3';

// Minimal-but-sniffable resource bytes (magic headers + padding).
const png = Uint8Array.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0, 0, 0, 13, 73, 72, 68, 82, 1, 2, 3, 4]);
const mp3 = Uint8Array.from([0x49, 0x44, 0x33, 4, 0, 0, 0, 0, 0, 0, 0xff, 0xfb, 0x90, 0x00, 9, 9, 9, 9]);
const resources = new Map([['icon1.png', png], ['tada.mp3', mp3]]);

const ssz = buildSsz(serializeSsc(board), resources, referencedResources(board));
writeFileSync(out, ssz);
console.log(`wrote ${out} (${ssz.length} bytes, ${resources.size} resources)`);
