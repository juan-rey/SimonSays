/**
 * validate.js — the "Validate board" checklist (format reference §10).
 * Produces [{ level: 'error'|'warning', code, detail, target }] where target
 * pinpoints the field: { kind: 'board'|'category'|'phrase'|'resource',
 * catIndex?, phraseIndex?, name? }. Message text is resolved by the UI layer
 * from the localized string table.
 */

import { categoryNameError, phraseTextError, iconTokenError, referencedResources } from './ssc.js';
import { isAllowedResourceName, extOf, EXT_KIND } from './sniff.js';

const AUDIO_EXTS = new Set(['wav', 'mp3']);
const ICON_EXTS = new Set(['ico', 'png', 'jpg', 'jpeg']);

export function validateBoard(board, resources, limits = {}) {
  const findings = [];
  const err = (code, target, detail = '') => findings.push({ level: 'error', code, detail, target });
  const warn = (code, target, detail = '') => findings.push({ level: 'warning', code, detail, target });

  if (board.categories.length === 0) {
    warn('empty-board', { kind: 'board' });
  }

  const seenNames = new Map();
  board.categories.forEach((c, ci) => {
    const tCat = { kind: 'category', catIndex: ci };

    const nameErr = categoryNameError(c.name);
    if (nameErr) err('category-name-' + nameErr, tCat, c.name);
    const key = c.name.trim();
    if (key) {
      if (seenNames.has(key)) err('category-name-duplicate', tCat, c.name);
      seenNames.set(key, ci);
    }

    const iconErr = iconTokenError(c.icon);
    if (iconErr) err('icon-' + iconErr, tCat, c.icon);
    checkIconRef(c.icon, tCat);

    if (c.phrases.length === 0) warn('empty-category', tCat, c.name);

    c.phrases.forEach((p, pi) => {
      const tPhr = { kind: 'phrase', catIndex: ci, phraseIndex: pi };
      const textErr = phraseTextError(p.text);
      if (textErr) err('phrase-text-' + textErr, tPhr, p.text);
      const pIconErr = iconTokenError(p.icon);
      if (pIconErr) err('icon-' + pIconErr, tPhr, p.icon);
      checkIconRef(p.icon, tPhr);
      if (p.audioFile) {
        if (!AUDIO_EXTS.has(extOf(p.audioFile))) err('audio-bad-extension', tPhr, p.audioFile);
        else if (!resources.has(p.audioFile)) warn('dangling-audio', tPhr, p.audioFile);
      }
    });
  });

  // Icon references: a token containing a dot is a file reference (the app's
  // emoji-vs-file heuristic); it must be a supported image and, ideally, be
  // present among the board's resources.
  function checkIconRef(icon, target) {
    if (!icon || !icon.includes('.')) return; // emoji or empty
    if (!ICON_EXTS.has(extOf(icon))) err('icon-bad-extension', target, icon);
    else if (!resources.has(icon)) warn('dangling-icon', target, icon);
  }

  // Resource-side checks.
  const referenced = referencedResources(board);
  let total = 0;
  for (const [name, bytes] of resources) {
    const tRes = { kind: 'resource', name };
    if (!isAllowedResourceName(name)) err('resource-bad-extension', tRes, name);
    if (limits.maxResourceBytes && bytes.length > limits.maxResourceBytes) {
      err('resource-too-large', tRes, name);
    }
    if (!referenced.has(name)) warn('unreferenced-resource', tRes, name);
    total += bytes.length;
  }
  if (limits.maxTotalBytes && total > limits.maxTotalBytes) {
    err('total-too-large', { kind: 'board' });
  }

  return findings;
}

export function hasBlockingErrors(findings) {
  return findings.some((f) => f.level === 'error');
}
