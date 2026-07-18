/**
 * zip.js — minimal, dependency-free Zip reader/writer for .ssz bundles.
 *
 * Writer: STORED entries only (method 0). Board resources are already
 * compressed formats (PNG/JPG/MP3) and the .ssc is tiny, so deflate would buy
 * little; STORED keeps this module free of compression code and the archive
 * fully readable by the SimonSays desktop app (miniz).
 *
 * Reader: parses the central directory and extracts STORED (0) and DEFLATE (8)
 * entries — the desktop app exports deflated entries — using the browser's
 * native DecompressionStream('deflate-raw'). CRC-32 and size are verified on
 * every extraction. ZIP64 archives are rejected (far beyond the format caps).
 *
 * Implements the container rules of docs/guides/ssc-ssz-format-reference.md §4.
 */

const LOCAL_SIG = 0x04034b50;
const CENTRAL_SIG = 0x02014b50;
const EOCD_SIG = 0x06054b50;

export class ZipError extends Error {
  constructor(code, detail = '') {
    super(detail ? `${code}: ${detail}` : code);
    this.name = 'ZipError';
    this.code = code;
    this.detail = detail;
  }
}

const CRC_TABLE = (() => {
  const t = new Uint32Array(256);
  for (let n = 0; n < 256; n++) {
    let c = n;
    for (let k = 0; k < 8; k++) c = (c & 1) ? (0xedb88320 ^ (c >>> 1)) : (c >>> 1);
    t[n] = c >>> 0;
  }
  return t;
})();

export function crc32(bytes) {
  let c = 0xffffffff;
  for (let i = 0; i < bytes.length; i++) c = CRC_TABLE[(c ^ bytes[i]) & 0xff] ^ (c >>> 8);
  return (c ^ 0xffffffff) >>> 0;
}

/** Default inflate for method-8 entries: native stream, no dependencies. */
export async function defaultInflateRaw(raw) {
  const ds = new DecompressionStream('deflate-raw');
  const stream = new Blob([raw]).stream().pipeThrough(ds);
  const buf = await new Response(stream).arrayBuffer();
  return new Uint8Array(buf);
}

/**
 * Builds a Zip from [{name, bytes}] with STORED entries and UTF-8 names.
 * Returns a Uint8Array of the whole archive.
 */
export function buildZip(entries) {
  const enc = new TextEncoder();
  const chunks = [];
  const central = [];
  let offset = 0;

  const now = new Date();
  const dosTime = ((now.getHours() << 11) | (now.getMinutes() << 5) | (now.getSeconds() >> 1)) & 0xffff;
  const dosDate = (((now.getFullYear() - 1980) << 9) | ((now.getMonth() + 1) << 5) | now.getDate()) & 0xffff;

  for (const { name, bytes } of entries) {
    const nameB = enc.encode(name);
    const crc = crc32(bytes);
    const lh = new DataView(new ArrayBuffer(30));
    lh.setUint32(0, LOCAL_SIG, true);
    lh.setUint16(4, 20, true);        // version needed to extract
    lh.setUint16(6, 0x0800, true);    // general purpose: UTF-8 names
    lh.setUint16(8, 0, true);         // method: STORED
    lh.setUint16(10, dosTime, true);
    lh.setUint16(12, dosDate, true);
    lh.setUint32(14, crc, true);
    lh.setUint32(18, bytes.length, true);
    lh.setUint32(22, bytes.length, true);
    lh.setUint16(26, nameB.length, true);
    lh.setUint16(28, 0, true);        // extra length
    chunks.push(new Uint8Array(lh.buffer), nameB, bytes);
    central.push({ nameB, crc, size: bytes.length, offset, dosTime, dosDate });
    offset += 30 + nameB.length + bytes.length;
  }

  const cdStart = offset;
  for (const e of central) {
    const ch = new DataView(new ArrayBuffer(46));
    ch.setUint32(0, CENTRAL_SIG, true);
    ch.setUint16(4, 20, true);        // version made by
    ch.setUint16(6, 20, true);        // version needed
    ch.setUint16(8, 0x0800, true);    // UTF-8 names
    ch.setUint16(10, 0, true);        // method: STORED
    ch.setUint16(12, e.dosTime, true);
    ch.setUint16(14, e.dosDate, true);
    ch.setUint32(16, e.crc, true);
    ch.setUint32(20, e.size, true);
    ch.setUint32(24, e.size, true);
    ch.setUint16(28, e.nameB.length, true);
    // extra(30), comment(32), disk(34), internal attrs(36) stay 0
    ch.setUint32(38, 0, true);        // external attrs (plain file)
    ch.setUint32(42, e.offset, true); // local header offset
    chunks.push(new Uint8Array(ch.buffer), e.nameB);
    offset += 46 + e.nameB.length;
  }

  const eocd = new DataView(new ArrayBuffer(22));
  eocd.setUint32(0, EOCD_SIG, true);
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

/**
 * Parses the central directory. Returns
 * [{name, method, crc, csize, usize, localOffset, externalAttr, isDirectory}].
 * Throws ZipError('not-a-zip' | 'corrupt' | 'zip64-unsupported').
 */
export function listEntries(bytes) {
  const dv = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  if (bytes.length < 22) throw new ZipError('not-a-zip');

  // Find the End Of Central Directory record, scanning back over a possible
  // trailing comment (max 65535 bytes).
  let eocd = -1;
  const scanStart = Math.max(0, bytes.length - 22 - 65535);
  for (let i = bytes.length - 22; i >= scanStart; i--) {
    if (dv.getUint32(i, true) === EOCD_SIG) { eocd = i; break; }
  }
  if (eocd < 0) throw new ZipError('not-a-zip');

  const count = dv.getUint16(eocd + 10, true);
  const cdSize = dv.getUint32(eocd + 12, true);
  const cdOfs = dv.getUint32(eocd + 16, true);
  if (count === 0xffff || cdOfs === 0xffffffff) throw new ZipError('zip64-unsupported');
  if (cdOfs + cdSize > bytes.length) throw new ZipError('corrupt');

  const dec = new TextDecoder('utf-8');
  const entries = [];
  let p = cdOfs;
  for (let i = 0; i < count; i++) {
    if (p + 46 > bytes.length || dv.getUint32(p, true) !== CENTRAL_SIG) throw new ZipError('corrupt');
    const method = dv.getUint16(p + 10, true);
    const crc = dv.getUint32(p + 16, true);
    const csize = dv.getUint32(p + 20, true);
    const usize = dv.getUint32(p + 24, true);
    const nameLen = dv.getUint16(p + 28, true);
    const extraLen = dv.getUint16(p + 30, true);
    const commentLen = dv.getUint16(p + 32, true);
    const externalAttr = dv.getUint32(p + 38, true);
    const localOffset = dv.getUint32(p + 42, true);
    if (csize === 0xffffffff || usize === 0xffffffff || localOffset === 0xffffffff) {
      throw new ZipError('zip64-unsupported');
    }
    if (p + 46 + nameLen > bytes.length) throw new ZipError('corrupt');
    const name = dec.decode(bytes.subarray(p + 46, p + 46 + nameLen));
    entries.push({ name, method, crc, csize, usize, localOffset, externalAttr, isDirectory: name.endsWith('/') });
    p += 46 + nameLen + extraLen + commentLen;
  }
  return entries;
}

/**
 * Extracts one entry (from listEntries) and verifies size + CRC-32.
 * Supports STORED (0) and DEFLATE (8). Returns a fresh Uint8Array.
 */
export async function extractEntry(bytes, entry, inflateRaw = defaultInflateRaw) {
  const dv = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const p = entry.localOffset;
  if (p + 30 > bytes.length || dv.getUint32(p, true) !== LOCAL_SIG) throw new ZipError('corrupt');
  // The local extra field may differ from the central one — read it locally.
  const nameLen = dv.getUint16(p + 26, true);
  const extraLen = dv.getUint16(p + 28, true);
  const start = p + 30 + nameLen + extraLen;
  if (start + entry.csize > bytes.length) throw new ZipError('corrupt');
  const raw = bytes.subarray(start, start + entry.csize);

  let data;
  if (entry.method === 0) data = raw.slice();
  else if (entry.method === 8) data = await inflateRaw(raw);
  else throw new ZipError('unsupported-method', String(entry.method));

  if (data.length !== entry.usize || crc32(data) !== entry.crc) throw new ZipError('corrupt', entry.name);
  return data;
}
