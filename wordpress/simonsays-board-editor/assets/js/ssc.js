/**
 * ssc.js — parse/serialize the SimonSays `.ssc` text format.
 *
 * Implements docs/guides/ssc-ssz-format-reference.md §2–3:
 *   EF BB BF (optional BOM)
 *   SIMONSAYS_CATEGORIES_V1\n
 *   $$board=$$<style list>\n                  (optional)
 *   <icon>##<name>=<$$style?|phrase|phrase…>\n
 *
 * Board model produced/consumed here:
 *   { boardStyle: string,                        // raw style list, verbatim
 *     categories: [ { icon, name, styleRaw,      // styleRaw verbatim
 *                     phrases: [ { icon, text, audioFile } ] } ] }
 */

export const SSC_HEADER = 'SIMONSAYS_CATEGORIES_V1';
export const BOARD_STYLE_NAME = '$$board';
export const STYLE_TOKEN_PREFIX = '$$';

export class SscError extends Error {
  constructor(code, detail = '') {
    super(detail ? `${code}: ${detail}` : code);
    this.name = 'SscError';
    this.code = code;
    this.detail = detail;
  }
}

/** Joins style lists, inserting ';' when the first lacks its trailing one. */
export function appendStyleList(style, addition) {
  if (!addition) return style;
  if (style && !style.endsWith(';')) style += ';';
  return style + addition;
}

/** `<icon>##<name>` with an optional `::<style>` suffix (mirrors the app). */
function parseCategoryHeader(left) {
  let icon = '';
  let rest = left;
  const iconPos = rest.indexOf('##');
  if (iconPos !== -1) {
    icon = rest.slice(0, iconPos);
    rest = rest.slice(iconPos + 2);
  }
  let styleRaw = '';
  const stylePos = rest.indexOf('::');
  if (stylePos !== -1) {
    styleRaw = rest.slice(stylePos + 2);
    rest = rest.slice(0, stylePos);
  }
  return { icon, name: rest, styleRaw };
}

/** One phrase token: `<icon>##<text>::<audioFile>` (icon and audio optional). */
export function parsePhraseToken(token) {
  let icon = '';
  let start = 0;
  const iconPos = token.indexOf('##');
  if (iconPos !== -1) {
    icon = token.slice(0, iconPos);
    start = iconPos + 2;
  }
  // Search the audio separator after the icon split (the desktop searches from
  // 0; the results differ only for inputs the desktop mangles anyway).
  const audioPos = token.indexOf('::', start);
  if (audioPos !== -1) {
    return { icon, text: token.slice(start, audioPos), audioFile: token.slice(audioPos + 2) };
  }
  return { icon, text: token.slice(start), audioFile: '' };
}

export function serializePhrase(phrase) {
  return (phrase.icon ? phrase.icon + '##' : '') + phrase.text +
    (phrase.audioFile ? '::' + phrase.audioFile : '');
}

/** Category data (right of `=`): `$$` tokens -> style, the rest -> phrases. */
function parseCategoryData(category, data) {
  for (const token of data.split('|')) {
    if (!token) continue;
    if (token.startsWith(STYLE_TOKEN_PREFIX)) {
      category.styleRaw = appendStyleList(category.styleRaw, token.slice(STYLE_TOKEN_PREFIX.length));
    } else {
      category.phrases.push(parsePhraseToken(token));
    }
  }
}

/** `$$board` data: tokens joined by `|`, each with an optional leading `$$`. */
function extractBoardStyleFromData(data) {
  let style = '';
  for (let token of data.split('|')) {
    if (!token) continue;
    if (token.startsWith(STYLE_TOKEN_PREFIX)) token = token.slice(STYLE_TOKEN_PREFIX.length);
    style = appendStyleList(style, token);
  }
  return style;
}

/**
 * Parses `.ssc` text into a board. Throws SscError('bad-header') when the
 * first line is not the exact format header.
 */
export function parseSsc(text) {
  if (text.startsWith('﻿')) text = text.slice(1);
  const lines = text.split('\n');
  const first = (lines[0] ?? '').replace(/\r$/, '');
  if (first !== SSC_HEADER) throw new SscError('bad-header');

  const board = { boardStyle: '', categories: [] };
  for (let i = 1; i < lines.length; i++) {
    const line = lines[i].replace(/\r$/, '');
    if (!line) continue;
    const sep = line.indexOf('=');
    if (sep === -1) continue; // skipped, per the format reference

    const left = line.slice(0, sep);
    const data = line.slice(sep + 1);
    if (left === BOARD_STYLE_NAME) {
      board.boardStyle = appendStyleList(board.boardStyle, extractBoardStyleFromData(data));
      continue;
    }
    const category = { ...parseCategoryHeader(left), phrases: [] };
    parseCategoryData(category, data);
    board.categories.push(category);
  }
  return board;
}

/** Serializes a board to `.ssc` text (LF, no BOM — see sscFileBytes). */
export function serializeSsc(board) {
  const lines = [SSC_HEADER];
  if (board.boardStyle) {
    lines.push(`${BOARD_STYLE_NAME}=${STYLE_TOKEN_PREFIX}${board.boardStyle}`);
  }
  for (const c of board.categories) {
    const left = (c.icon ? c.icon + '##' : '') + c.name;
    let data = c.styleRaw ? STYLE_TOKEN_PREFIX + c.styleRaw : '';
    for (const p of c.phrases) {
      const token = serializePhrase(p);
      data = data ? data + '|' + token : token;
    }
    lines.push(left + '=' + data);
  }
  return lines.join('\n') + '\n';
}

/** The exact on-disk bytes for a `.ssc` download: UTF-8 BOM + text. */
export function sscFileBytes(text) {
  return new TextEncoder().encode('﻿' + text);
}

// ---------------------------------------------------------------------------
// Reserved-marker validation (format reference §3 / §10 checklist)
// ---------------------------------------------------------------------------

/**
 * Category names must not carry any structural marker. Returns an error code
 * or null. `♫` is rejected here (meaningless in a name); note it IS legal in
 * phrase text, where it is the inline-sound marker.
 */
export function categoryNameError(name) {
  if (!name || !name.trim()) return 'empty';
  if (name.startsWith(STYLE_TOKEN_PREFIX)) return 'reserved-prefix';
  if (name.includes('|')) return 'char-pipe';
  if (name.includes('##')) return 'char-icon-sep';
  if (name.includes('::')) return 'char-audio-sep';
  if (name.includes('=')) return 'char-equals';
  if (name.includes('♫')) return 'char-note';
  return null;
}

/**
 * Phrase text: the markers that would break tokenization are rejected; `♫`
 * stays allowed on purpose — it is the documented inline sound-marker syntax.
 */
export function phraseTextError(text) {
  if (!text || !text.trim()) return 'empty';
  if (text.includes('|')) return 'char-pipe';
  if (text.includes('##')) return 'char-icon-sep';
  if (text.includes('::')) return 'char-audio-sep';
  return null;
}

/** Icon tokens (emoji or bare filename) share the phrase constraints. */
export function iconTokenError(icon) {
  if (!icon) return null; // optional
  if (icon.includes('|')) return 'char-pipe';
  if (icon.includes('##')) return 'char-icon-sep';
  if (icon.includes('::')) return 'char-audio-sep';
  return null;
}

/** Style free-text values (caption, title, …): no `;`, no `|`. */
export function styleFreeTextError(value) {
  if (value.includes(';')) return 'char-semicolon';
  if (value.includes('|')) return 'char-pipe';
  return null;
}

/**
 * Renames a resource across every board reference (category icons, phrase
 * icons, phrase audio). Returns the number of references updated.
 */
export function renameResourceReferences(board, oldName, newName) {
  let updated = 0;
  const swap = (obj, field) => {
    if (obj[field] === oldName) {
      obj[field] = newName;
      updated++;
    }
  };
  for (const c of board.categories) {
    swap(c, 'icon');
    for (const p of c.phrases) {
      swap(p, 'icon');
      swap(p, 'audioFile');
    }
  }
  return updated;
}

/**
 * Distinct resource basenames referenced by the board: file icons (contain a
 * dot, per the app's emoji-vs-file heuristic) plus phrase audio files.
 */
export function referencedResources(board) {
  const refs = new Set();
  const consider = (v) => { if (v && v.includes('.')) refs.add(v); };
  for (const c of board.categories) {
    consider(c.icon);
    for (const p of c.phrases) {
      consider(p.icon);
      if (p.audioFile) refs.add(p.audioFile);
    }
  }
  return refs;
}
