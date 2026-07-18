/**
 * sniff.js — magic-byte detection for the allowed resource types.
 * The whitelist mirrors the .ssz bundle contract: .ico .png .jpg/.jpeg .wav .mp3
 * (never SVG, never extension-only trust).
 */

export const EXT_KIND = { ico: 'ico', png: 'png', jpg: 'jpg', jpeg: 'jpg', wav: 'wav', mp3: 'mp3' };

export function extOf(name) {
  const dot = name.lastIndexOf('.');
  return dot === -1 ? '' : name.slice(dot + 1).toLowerCase();
}

export function isAllowedResourceName(name) {
  return extOf(name) in EXT_KIND;
}

/** Detects the content kind from magic bytes, or returns null. */
export function sniff(bytes) {
  if (!bytes || bytes.length < 4) return null;
  const b = bytes;
  // PNG: 89 50 4E 47 0D 0A 1A 0A
  if (b.length >= 8 && b[0] === 0x89 && b[1] === 0x50 && b[2] === 0x4e && b[3] === 0x47 &&
    b[4] === 0x0d && b[5] === 0x0a && b[6] === 0x1a && b[7] === 0x0a) return 'png';
  // JPEG: FF D8 FF
  if (b[0] === 0xff && b[1] === 0xd8 && b[2] === 0xff) return 'jpg';
  // ICO: 00 00 01 00 (icon directory; 02 would be a .cur cursor — not accepted)
  if (b[0] === 0x00 && b[1] === 0x00 && b[2] === 0x01 && b[3] === 0x00) return 'ico';
  // WAV: "RIFF" .... "WAVE"
  if (b.length >= 12 && b[0] === 0x52 && b[1] === 0x49 && b[2] === 0x46 && b[3] === 0x46 &&
    b[8] === 0x57 && b[9] === 0x41 && b[10] === 0x56 && b[11] === 0x45) return 'wav';
  // MP3: "ID3" tag, or a bare MPEG frame sync (FF Ex/Fx)
  if (b[0] === 0x49 && b[1] === 0x44 && b[2] === 0x33) return 'mp3';
  if (b[0] === 0xff && (b[1] & 0xe0) === 0xe0) return 'mp3';
  return null;
}

/** True when the sniffed kind is consistent with the file's extension. */
export function kindMatchesExt(kind, ext) {
  return kind !== null && EXT_KIND[ext] === kind;
}

/** MIME for preview blob URLs, derived from the (already validated) extension. */
export function mimeForExt(ext) {
  switch (EXT_KIND[ext]) {
    case 'png': return 'image/png';
    case 'jpg': return 'image/jpeg';
    case 'ico': return 'image/x-icon';
    case 'wav': return 'audio/wav';
    case 'mp3': return 'audio/mpeg';
    default: return 'application/octet-stream';
  }
}
