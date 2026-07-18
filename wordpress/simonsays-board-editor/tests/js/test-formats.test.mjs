// Format-core tests: ssc.js, style.js, sniff.js, validate.js  (node --test)
import { test } from 'node:test';
import assert from 'node:assert/strict';

import {
  parseSsc, serializeSsc, sscFileBytes, SscError,
  categoryNameError, phraseTextError, styleFreeTextError, referencedResources,
  renameResourceReferences,
} from '../../assets/js/ssc.js';
import { sanitizePictogramName, uniquePictogramName } from '../../assets/js/arasaac.js';
import {
  parseBoardStyle, parseCategoryStyle, resolveEffective, resolveSize,
  contrastColor, upsertPair, removePairs, getPair, emptyProps,
} from '../../assets/js/style.js';
import { sniff, kindMatchesExt, extOf } from '../../assets/js/sniff.js';
import { validateBoard, hasBlockingErrors } from '../../assets/js/validate.js';

// The SPC adult board example from docs/guides/board-style-reference.md §9.
const SPC = [
  'SIMONSAYS_CATEGORIES_V1',
  '$$board=$$background:#FBFAFD;separator-color:#B4A7D6;category-icon-position:top;phrase-corner-radius:8;caption:SPC Board;title:SPC Adult Board;credits:by SimonSays;',
  '🧍##Personas=$$background:#FFD966;|Yo|Tú|Familia|Cuidador|Médico',
  '🏃##Acciones=$$background:#93C47D;|Quiero|Necesito|Comer|Beber|Ir|Sentir',
  '📏##Descriptivos=$$background:#6FABDC;|Mucho|Poco|Bueno|Malo|Frío|Caliente',
  '🍎##Sustantivos=$$background:#F6B26B;|Agua|Comida|Baño|Teléfono|Medicación|Dolor',
  '🕒##Miscelánea=$$background:#FFFFFF;|Hoy|Mañana|Aquí|Allí|Números|Colores',
  '👋##Social=$$background:#B4A7D6;|Hola|Gracias|Por favor|Sí|No|Adiós',
].join('\n') + '\n';

test('ssc: SPC example round-trips byte-identically', () => {
  const board = parseSsc(SPC);
  assert.equal(board.categories.length, 6);
  assert.equal(board.categories[0].icon, '🧍');
  assert.equal(board.categories[0].name, 'Personas');
  assert.equal(board.categories[0].styleRaw, 'background:#FFD966;');
  assert.equal(board.categories[0].phrases.length, 5);
  assert.match(board.boardStyle, /caption:SPC Board;/);
  assert.equal(serializeSsc(board), SPC);
});

test('ssc: BOM + CRLF accepted; wrong header rejected; no-= lines skipped', () => {
  const withBom = '﻿' + SPC.replaceAll('\n', '\r\n');
  const board = parseSsc(withBom);
  assert.equal(board.categories.length, 6);
  assert.throws(() => parseSsc('NOT_A_HEADER\nGreet=Hi\n'), SscError);
  const skipped = parseSsc('SIMONSAYS_CATEGORIES_V1\nno separator line\nGreet=Hi\n');
  assert.equal(skipped.categories.length, 1);
});

test('ssc: phrase grammar (icon, audio, both) and unknown-style token order', () => {
  const text = 'SIMONSAYS_CATEGORIES_V1\n' +
    '🙂##Greet=$$background:#93C47D;future-prop:1;|👋##Hello|Bye::bye.mp3|🔔##Ring::chime.wav\n';
  const b = parseSsc(text);
  const c = b.categories[0];
  assert.equal(c.styleRaw, 'background:#93C47D;future-prop:1;');
  assert.deepEqual(c.phrases[0], { icon: '👋', text: 'Hello', audioFile: '' });
  assert.deepEqual(c.phrases[1], { icon: '', text: 'Bye', audioFile: 'bye.mp3' });
  assert.deepEqual(c.phrases[2], { icon: '🔔', text: 'Ring', audioFile: 'chime.wav' });
  assert.equal(serializeSsc(b), text); // unknown style pair preserved verbatim
});

test('ssc: reserved-marker validation', () => {
  assert.equal(categoryNameError('People'), null);
  assert.equal(categoryNameError('$$board'), 'reserved-prefix');
  assert.equal(categoryNameError('a|b'), 'char-pipe');
  assert.equal(categoryNameError('a##b'), 'char-icon-sep');
  assert.equal(categoryNameError('a=b'), 'char-equals');
  assert.equal(categoryNameError('a♫b'), 'char-note');
  assert.equal(phraseTextError('Play ♫tada.mp3♫ now'), null); // inline sound marker is legal
  assert.equal(phraseTextError('a|b'), 'char-pipe');
  assert.equal(phraseTextError('a::b'), 'char-audio-sep');
  assert.equal(styleFreeTextError('My Board'), null);
  assert.equal(styleFreeTextError('a;b'), 'char-semicolon');
});

test('ssc: sscFileBytes starts with the UTF-8 BOM', () => {
  const bytes = sscFileBytes('SIMONSAYS_CATEGORIES_V1\n');
  assert.deepEqual([...bytes.slice(0, 3)], [0xef, 0xbb, 0xbf]);
});

test('style: parsing rules — trim, case, skip-on-error, last-wins', () => {
  const s = parseBoardStyle('  BackGround : #ff0000 ; nope ; bad-color:#f00 ; background:#00ff00 ; phrase-height:120% ; category-icon-position:top');
  assert.equal(s.window.background, '#00ff00'); // last wins; case/space tolerated
  assert.deepEqual(s.phraseButtons.height, { value: 120, isPercent: true });
  assert.equal(s.categoryButtons.iconPosition, 'top');
});

test('style: window vs group scope routing', () => {
  const s = parseBoardStyle('separator-color:#123456;width:50;caption:Hi;phrase-width:80;category-caption:x;');
  assert.equal(s.window.separatorColor, '#123456');
  assert.equal(s.window.width, null);               // geometry rejected in window scope
  assert.equal(s.window.caption, 'Hi');
  assert.deepEqual(s.phraseButtons.width, { value: 80, isPercent: false });
  assert.equal(s.categoryButtons.caption, '');      // free text rejected in group scope
});

test('style: category layer accepts only own colors + phrase-*', () => {
  const c = parseCategoryStyle('background:#FFD966;text-color:#000000;width:50;phrase-corner-radius:8;');
  assert.equal(c.ownButton.background, '#ffd966');
  assert.equal(c.ownButton.textColor, '#000000');
  assert.equal(c.ownButton.width, null);
  assert.deepEqual(c.phraseButtons.cornerRadius, { value: 8, isPercent: false });
});

test('style: text-layout smart/axes and cascade atomicity', () => {
  const smart = parseBoardStyle('phrase-text-layout:smart;');
  assert.equal(smart.phraseButtons.textLayoutSmart, true);
  const axes = parseBoardStyle('phrase-text-layout:top center;');
  assert.equal(axes.phraseButtons.textVAlign, 'top');
  assert.equal(axes.phraseButtons.textHAlign, 'center');

  const base = axes.phraseButtons;
  const over = smart.phraseButtons;
  const r = resolveEffective(base, over);           // smart resets inherited axes
  assert.equal(r.textLayoutSmart, true);
  assert.equal(r.textHAlign, '');
  const r2 = resolveEffective(over, axes.phraseButtons); // explicit drops smart
  assert.equal(r2.textLayoutSmart, false);
  assert.equal(r2.textVAlign, 'top');
});

test('style: sizes, weight, contrast', () => {
  assert.equal(resolveSize({ value: 130, isPercent: true }, 40), 52);
  assert.equal(resolveSize({ value: 0, isPercent: false }, 40), 40);       // zero ignored
  assert.equal(resolveSize({ value: 0, isPercent: false }, 40, true), 0); // allowZero
  assert.equal(resolveSize(null, 40), 40);
  const w = parseBoardStyle('text-weight:600;');
  assert.equal(w.window.textWeight, 600);
  assert.equal(parseBoardStyle('text-weight:950;').window.textWeight, 0);
  assert.equal(contrastColor('#ffffff'), '#000000');
  assert.equal(contrastColor('#292929'), '#ffffff');
});

test('style: upsertPair/removePairs preserve unknown segments verbatim', () => {
  const raw = 'background:#FFD966;future-prop:1;malformed segment;';
  const up = upsertPair(raw, 'background', '#00ff00');
  assert.equal(up, 'background:#00ff00;future-prop:1;malformed segment;');
  const added = upsertPair(up, 'phrase-height', '120%');
  assert.equal(getPair(added, 'phrase-height'), '120%');
  assert.match(added, /future-prop:1;/);
  const removed = removePairs(added, 'background');
  assert.equal(getPair(removed, 'background'), null);
  assert.match(removed, /malformed segment/);
});

test('style: resource-folder and title parse as window free text', () => {
  const s = parseBoardStyle('title:My Board;resource-folder:Shared Pack;');
  assert.equal(s.window.title, 'My Board');
  assert.equal(s.window.resourceFolder, 'Shared Pack');
});

test('sniff: magic bytes for every allowed kind, SVG/none rejected', () => {
  const png = Uint8Array.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0, 0]);
  const jpg = Uint8Array.from([0xff, 0xd8, 0xff, 0xe0, 0, 0]);
  const ico = Uint8Array.from([0, 0, 1, 0, 1, 0]);
  const wav = Uint8Array.from([...'RIFF'].map(c => c.charCodeAt(0)).concat([0, 0, 0, 0], [...'WAVE'].map(c => c.charCodeAt(0))));
  const mp3a = Uint8Array.from([...'ID3'].map(c => c.charCodeAt(0)).concat([4, 0, 0]));
  const mp3b = Uint8Array.from([0xff, 0xfb, 0x90, 0x00]);
  const svg = new TextEncoder().encode('<svg xmlns="http://www.w3.org/2000/svg"/>');
  assert.equal(sniff(png), 'png');
  assert.equal(sniff(jpg), 'jpg');
  assert.equal(sniff(ico), 'ico');
  assert.equal(sniff(wav), 'wav');
  assert.equal(sniff(mp3a), 'mp3');
  assert.equal(sniff(mp3b), 'mp3');
  assert.equal(sniff(svg), null);
  assert.ok(kindMatchesExt('jpg', extOf('photo.JPEG')));
  assert.ok(!kindMatchesExt('png', 'mp3'));
});

test('validate: dangling refs, duplicates, unreferenced resources, caps', () => {
  const board = parseSsc('SIMONSAYS_CATEGORIES_V1\nA=x::gone.mp3|ok.png##y\nA=z\n');
  const resources = new Map([
    ['ok.png', Uint8Array.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a])],
    ['unused.mp3', Uint8Array.from([0xff, 0xfb, 0, 0])],
  ]);
  const findings = validateBoard(board, resources, { maxResourceBytes: 4 });
  const codes = findings.map(f => f.code);
  assert.ok(codes.includes('category-name-duplicate'));
  assert.ok(codes.includes('dangling-audio'));
  assert.ok(codes.includes('unreferenced-resource'));
  assert.ok(codes.includes('resource-too-large'));
  assert.ok(hasBlockingErrors(findings));
  assert.deepEqual([...referencedResources(board)], ['gone.mp3', 'ok.png']);
});

test('ssc: renameResourceReferences updates every reference kind', () => {
  const board = parseSsc('SIMONSAYS_CATEGORIES_V1\n' +
    'a.png##Cat=a.png##One::a.png|Two::b.mp3|a.png##Three\n' +
    'Other=Keep::a.png\n');
  // NOTE: a.png as audio is not realistic but exercises the field swap.
  const n = renameResourceReferences(board, 'a.png', 'renamed.png');
  assert.equal(n, 5); // cat icon + 2 phrase icons + 2 audio fields
  assert.equal(board.categories[0].icon, 'renamed.png');
  assert.equal(board.categories[0].phrases[0].icon, 'renamed.png');
  assert.equal(board.categories[0].phrases[0].audioFile, 'renamed.png');
  assert.equal(board.categories[0].phrases[1].audioFile, 'b.mp3');
  assert.equal(board.categories[1].phrases[0].audioFile, 'renamed.png');
});

test('arasaac: pictogram naming from search text', () => {
  assert.equal(sanitizePictogramName('agua'), 'agua');
  assert.equal(sanitizePictogramName('beber agua'), 'beber_agua');
  assert.equal(sanitizePictogramName('  a|b##c::d=e;f♫g  '), 'abcdefg');
  assert.equal(sanitizePictogramName('..\\..\\evil'), 'evil');
  assert.equal(sanitizePictogramName('???'), '');
  assert.equal(sanitizePictogramName('a.b.c'), 'a_b_c');
  assert.ok(sanitizePictogramName('x'.repeat(80)).length <= 40);
  const taken = new Set(['agua.png', 'agua_2.png']);
  assert.equal(uniquePictogramName('agua', taken), 'agua_3.png');
  assert.equal(uniquePictogramName('sol', taken), 'sol.png');
});

test('validate: clean board has no errors', () => {
  const board = parseSsc('SIMONSAYS_CATEGORIES_V1\n🙂##Greet=Hello|Bye::bye.mp3\n');
  const resources = new Map([['bye.mp3', Uint8Array.from([0xff, 0xfb, 0, 0])]]);
  const findings = validateBoard(board, resources, {});
  assert.ok(!hasBlockingErrors(findings));
});
