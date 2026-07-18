/**
 * ssz.js — read/write the `.ssz` bundle with the hardening rules of
 * docs/guides/ssc-ssz-format-reference.md §4–6:
 *   exactly one root categories.ssc; flat resources/ with whitelisted
 *   extensions; entry-name safety; symlink rejection; entry/total/ratio caps;
 *   content sniffed against the extension. Admin-configured size limits are
 *   applied on top of (never above) the format caps.
 */

import { buildZip, listEntries, extractEntry, ZipError } from './zip.js';
import { extOf, EXT_KIND, sniff, kindMatchesExt } from './sniff.js';

export const FORMAT_LIMITS = {
  maxEntries: 5000,
  maxEntryBytes: 64 * 1024 * 1024,
  maxTotalBytes: 256 * 1024 * 1024,
  maxRatio: 100,
};

export const SSC_ENTRY = 'categories.ssc';
export const RES_PREFIX = 'resources/';

export class SszError extends Error {
  constructor(code, detail = '') {
    super(detail ? `${code}: ${detail}` : code);
    this.name = 'SszError';
    this.code = code;
    this.detail = detail;
  }
}

/** Zip entry-name safety: reject traversal / absolute / drive / control chars. */
export function isSafeEntryName(name) {
  if (!name) return false;
  if (name.startsWith('/')) return false;
  if (name.includes('\\') || name.includes(':') || name.includes('..')) return false;
  for (const ch of name) {
    if (ch.codePointAt(0) < 0x20) return false;
  }
  return true;
}

/** Sniffs "PK\x03\x04" — mirrors how the app routes .ssz vs .ssc readers. */
export function isZipBytes(bytes) {
  return bytes.length >= 4 && bytes[0] === 0x50 && bytes[1] === 0x4b && bytes[2] === 0x03 && bytes[3] === 0x04;
}

/**
 * Validates and unpacks a bundle. Throws SszError with a stable `code` on any
 * violation; on success returns { sscText, resources: Map<name, Uint8Array> }.
 *
 * opts: { maxResourceBytes?, maxTotalBytes?, inflateRaw? } — admin caps are
 * clamped to the format caps.
 */
export async function parseSsz(bytes, opts = {}) {
  const maxFile = Math.min(opts.maxResourceBytes ?? Infinity, FORMAT_LIMITS.maxEntryBytes);
  const maxTotal = Math.min(opts.maxTotalBytes ?? Infinity, FORMAT_LIMITS.maxTotalBytes);

  let entries;
  try {
    entries = listEntries(bytes);
  } catch (e) {
    throw new SszError(e instanceof ZipError ? e.code : 'not-a-zip');
  }
  if (entries.length === 0 || entries.length > FORMAT_LIMITS.maxEntries) {
    throw new SszError('entry-count');
  }

  let sscEntry = null;
  const resourceEntries = new Map();
  let total = 0;

  for (const e of entries) {
    if (e.isDirectory) continue; // directory placeholders are ignored
    // Symlinks: Unix mode lives in the high 16 bits of the external attrs.
    if (((e.externalAttr >>> 16) & 0xf000) === 0xa000) throw new SszError('symlink', e.name);
    if (!isSafeEntryName(e.name)) throw new SszError('unsafe-name', e.name);
    if (e.usize > FORMAT_LIMITS.maxEntryBytes) throw new SszError('entry-too-large', e.name);
    total += e.usize;
    if (total > maxTotal) throw new SszError('total-too-large');
    if (e.csize > 0 && e.usize / e.csize > FORMAT_LIMITS.maxRatio) throw new SszError('ratio', e.name);

    if (e.name === SSC_ENTRY) {
      if (sscEntry) throw new SszError('multiple-ssc');
      sscEntry = e;
    } else if (e.name.startsWith(RES_PREFIX)) {
      const leaf = e.name.slice(RES_PREFIX.length);
      if (!leaf || leaf.includes('/')) throw new SszError('nested-resource', e.name);
      if (!(extOf(leaf) in EXT_KIND)) throw new SszError('bad-extension', leaf);
      if (e.usize > maxFile) throw new SszError('resource-too-large', leaf);
      if (resourceEntries.has(leaf)) throw new SszError('duplicate-resource', leaf);
      resourceEntries.set(leaf, e);
    } else {
      throw new SszError('unexpected-entry', e.name);
    }
  }
  if (!sscEntry) throw new SszError('no-ssc');

  let sscText;
  try {
    sscText = new TextDecoder('utf-8').decode(await extractEntry(bytes, sscEntry, opts.inflateRaw));
  } catch {
    throw new SszError('extract-failed', SSC_ENTRY);
  }

  const resources = new Map();
  for (const [leaf, e] of resourceEntries) {
    let data;
    try {
      data = await extractEntry(bytes, e, opts.inflateRaw);
    } catch {
      throw new SszError('extract-failed', leaf);
    }
    // Content must match the claimed extension (stricter than the desktop —
    // a web editor should not carry mislabeled bytes forward).
    if (!kindMatchesExt(sniff(data), extOf(leaf))) throw new SszError('content-mismatch', leaf);
    resources.set(leaf, data);
  }

  return { sscText, resources };
}

/**
 * Builds a bundle: BOM + sscText as categories.ssc, plus every resource whose
 * basename is in `referenced` (bare-basename policy — never bundle unused
 * files). Throws SszError('total-too-large') over the configured cap.
 */
export function buildSsz(sscText, resources, referenced, opts = {}) {
  const sscBytes = new TextEncoder().encode('﻿' + sscText);
  const entries = [{ name: SSC_ENTRY, bytes: sscBytes }];
  let total = sscBytes.length;

  const names = [...resources.keys()].filter((n) => referenced.has(n)).sort();
  for (const n of names) {
    const b = resources.get(n);
    total += b.length;
    entries.push({ name: RES_PREFIX + n, bytes: b });
  }

  const maxTotal = Math.min(opts.maxTotalBytes ?? Infinity, FORMAT_LIMITS.maxTotalBytes);
  if (total > maxTotal) throw new SszError('total-too-large');
  return buildZip(entries);
}
