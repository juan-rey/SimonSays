// Bundle tests: zip.js + ssz.js round-trips and hostile fixtures  (node --test)
import { test } from 'node:test';
import assert from 'node:assert/strict';
import { deflateRawSync } from 'node:zlib';

import { buildZip, listEntries, extractEntry, crc32, ZipError } from '../../assets/js/zip.js';
import { parseSsz, buildSsz, SszError, isSafeEntryName, isZipBytes, FORMAT_LIMITS } from '../../assets/js/ssz.js';
import { parseSsc, serializeSsc } from '../../assets/js/ssc.js';

const enc = new TextEncoder();
const PNG = Uint8Array.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 1, 2, 3]);
const MP3 = Uint8Array.from([0x49, 0x44, 0x33, 4, 0, 0, 9, 9]);
const SSC_TEXT = 'SIMONSAYS_CATEGORIES_V1\n🙂##Greet=icon.png##Hello|Bye::bye.mp3\n';

/**
 * Test-only zip builder able to write DEFLATE entries and arbitrary external
 * attributes — for crafting hostile fixtures the production writer refuses.
 */
function makeZip(entries) {
  const chunks = [];
  const central = [];
  let offset = 0;
  for (const e of entries) {
    const nameB = enc.encode(e.name);
    const data = e.bytes ?? new Uint8Array(0);
    const method = e.method ?? 0;
    const stored = method === 0 ? data : deflateRawSync(data);
    const lh = new DataView(new ArrayBuffer(30));
    lh.setUint32(0, 0x04034b50, true);
    lh.setUint16(4, 20, true);
    lh.setUint16(6, 0x0800, true);
    lh.setUint16(8, method, true);
    lh.setUint32(14, crc32(data), true);
    lh.setUint32(18, stored.length, true);
    lh.setUint32(22, data.length, true);
    lh.setUint16(26, nameB.length, true);
    chunks.push(new Uint8Array(lh.buffer), nameB, stored);
    central.push({ nameB, crc: crc32(data), csize: stored.length, usize: data.length, offset, method, extAttr: e.extAttr ?? 0 });
    offset += 30 + nameB.length + stored.length;
  }
  const cdStart = offset;
  for (const e of central) {
    const ch = new DataView(new ArrayBuffer(46));
    ch.setUint32(0, 0x02014b50, true);
    ch.setUint16(4, 20, true);
    ch.setUint16(6, 20, true);
    ch.setUint16(8, 0x0800, true);
    ch.setUint16(10, e.method, true);
    ch.setUint32(16, e.crc, true);
    ch.setUint32(20, e.csize, true);
    ch.setUint32(24, e.usize, true);
    ch.setUint16(28, e.nameB.length, true);
    ch.setUint32(38, e.extAttr, true);
    ch.setUint32(42, e.offset, true);
    chunks.push(new Uint8Array(ch.buffer), e.nameB);
    offset += 46 + e.nameB.length;
  }
  const eocd = new DataView(new ArrayBuffer(22));
  eocd.setUint32(0, 0x06054b50, true);
  eocd.setUint16(8, central.length, true);
  eocd.setUint16(10, central.length, true);
  eocd.setUint32(12, offset - cdStart, true);
  eocd.setUint32(16, cdStart, true);
  chunks.push(new Uint8Array(eocd.buffer));
  let total = 0;
  for (const c of chunks) total += c.length;
  const out = new Uint8Array(total);
  let p = 0;
  for (const c of chunks) { out.set(c, p); p += c.length; }
  return out;
}

async function expectSszError(bytes, code, opts = {}) {
  await assert.rejects(parseSsz(bytes, opts), (e) => e instanceof SszError && e.code === code, `expected SszError ${code}`);
}

test('zip: STORED round-trip with UTF-8 names', async () => {
  const zip = buildZip([
    { name: 'categories.ssc', bytes: enc.encode(SSC_TEXT) },
    { name: 'resources/año-ñ.png', bytes: PNG },
  ]);
  assert.ok(isZipBytes(zip));
  const entries = listEntries(zip);
  assert.equal(entries.length, 2);
  assert.equal(entries[1].name, 'resources/año-ñ.png');
  const data = await extractEntry(zip, entries[1]);
  assert.deepEqual([...data], [...PNG]);
});

test('zip: DEFLATE entries extract via DecompressionStream and verify CRC', async () => {
  const body = enc.encode(SSC_TEXT.repeat(50));
  const zip = makeZip([{ name: 'a.txt', bytes: body, method: 8 }]);
  const [e] = listEntries(zip);
  assert.equal(e.method, 8);
  const data = await extractEntry(zip, e);
  assert.deepEqual([...data], [...body]);
});

test('zip: corrupt data fails the CRC check', async () => {
  const zip = buildZip([{ name: 'a.bin', bytes: Uint8Array.from([1, 2, 3, 4, 5]) }]);
  // Stored data lives right after the 30-byte local header + 5-byte name.
  zip[30 + 5 + 2] ^= 0xff;
  const [e] = listEntries(zip);
  await assert.rejects(extractEntry(zip, e), ZipError);
});

test('zip: garbage is not-a-zip', () => {
  assert.throws(() => listEntries(enc.encode('this is not a zip file at all........')), ZipError);
});

test('ssz: build → parse round-trip; only referenced resources bundled', async () => {
  const resources = new Map([
    ['icon.png', PNG],
    ['bye.mp3', MP3],
    ['unused.png', PNG],
  ]);
  const referenced = new Set(['icon.png', 'bye.mp3']);
  const ssz = buildSsz(SSC_TEXT, resources, referenced);
  const { sscText, resources: out } = await parseSsz(ssz);
  assert.equal(sscText.replace(/^﻿/, ''), SSC_TEXT);
  assert.deepEqual([...out.keys()].sort(), ['bye.mp3', 'icon.png']);
  const board = parseSsc(sscText);
  assert.equal(serializeSsc(board), SSC_TEXT); // AC1: content-equivalent round-trip
});

test('ssz: hostile entry names are rejected', async () => {
  const ssc = { name: 'categories.ssc', bytes: enc.encode(SSC_TEXT) };
  await expectSszError(makeZip([ssc, { name: '../evil.png', bytes: PNG }]), 'unsafe-name');
  await expectSszError(makeZip([ssc, { name: 'resources/../../evil.png', bytes: PNG }]), 'unsafe-name');
  await expectSszError(makeZip([ssc, { name: 'resources\\evil.png', bytes: PNG }]), 'unsafe-name');
  await expectSszError(makeZip([ssc, { name: 'C:/evil.png', bytes: PNG }]), 'unsafe-name');
  await expectSszError(makeZip([ssc, { name: '/abs.png', bytes: PNG }]), 'unsafe-name');
});

test('ssz: structure violations are rejected', async () => {
  const ssc = { name: 'categories.ssc', bytes: enc.encode(SSC_TEXT) };
  await expectSszError(makeZip([{ name: 'resources/a.png', bytes: PNG }]), 'no-ssc');
  await expectSszError(makeZip([ssc, ssc]), 'multiple-ssc');
  await expectSszError(makeZip([ssc, { name: 'resources/sub/a.png', bytes: PNG }]), 'nested-resource');
  await expectSszError(makeZip([ssc, { name: 'resources/a.svg', bytes: PNG }]), 'bad-extension');
  await expectSszError(makeZip([ssc, { name: 'evil.exe', bytes: PNG }]), 'unexpected-entry');
  await expectSszError(makeZip([ssc, { name: 'resources/a.png', bytes: PNG, extAttr: 0xa1ff0000 }]), 'symlink');
});

test('ssz: zip bomb (ratio) and size caps are rejected', async () => {
  const ssc = { name: 'categories.ssc', bytes: enc.encode(SSC_TEXT) };
  // 2 MB of zeros deflates to a few KB → ratio far above 100.
  const zeros = new Uint8Array(2 * 1024 * 1024);
  await expectSszError(makeZip([ssc, { name: 'resources/bomb.png', bytes: zeros, method: 8 }]), 'ratio');
  // Admin cap: per-resource limit below the PNG's size.
  await expectSszError(makeZip([ssc, { name: 'resources/a.png', bytes: PNG }]), 'resource-too-large', { maxResourceBytes: 4 });
  // Admin cap: total limit below the archive content.
  await expectSszError(makeZip([ssc, { name: 'resources/a.png', bytes: PNG }]), 'total-too-large', { maxTotalBytes: 10 });
  assert.throws(
    () => buildSsz(SSC_TEXT, new Map([['a.png', PNG]]), new Set(['a.png']), { maxTotalBytes: 10 }),
    (e) => e instanceof SszError && e.code === 'total-too-large',
  );
});

test('ssz: content must match the claimed extension', async () => {
  const ssc = { name: 'categories.ssc', bytes: enc.encode(SSC_TEXT) };
  await expectSszError(makeZip([ssc, { name: 'resources/fake.png', bytes: MP3 }]), 'content-mismatch');
});

test('ssz: entry-name safety helper', () => {
  assert.ok(isSafeEntryName('resources/a.png'));
  assert.ok(!isSafeEntryName('a\\b.png'));
  assert.ok(!isSafeEntryName('a:b.png'));
  assert.ok(!isSafeEntryName('../a.png'));
  assert.ok(!isSafeEntryName('/a.png'));
  assert.ok(!isSafeEntryName('a\x01b.png')); // control character
  assert.ok(!isSafeEntryName(''));
});

test('ssz: format caps are the outer bound for admin settings', () => {
  assert.equal(FORMAT_LIMITS.maxEntries, 5000);
  assert.equal(FORMAT_LIMITS.maxEntryBytes, 64 * 1024 * 1024);
  assert.equal(FORMAT_LIMITS.maxTotalBytes, 256 * 1024 * 1024);
  assert.equal(FORMAT_LIMITS.maxRatio, 100);
});
