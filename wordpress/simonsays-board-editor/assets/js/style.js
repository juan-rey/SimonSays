/**
 * style.js — SimonSays board/category style lists.
 *
 * Implements docs/guides/board-style-reference.md:
 *   §2 syntax  (';' pairs, first ':', trim, case-insensitive names,
 *               skip-on-error, last-wins)
 *   §3 value forms (#RRGGBB, px / NN%, weight, icon-position, text-layout,
 *               free text)
 *   §4 cascade (built-in → board → category; own-color flow-through is the
 *               caller's job, resolveEffective is the per-property merge)
 *   §5 property routing (window scope vs category-/phrase- groups)
 *
 * Editing model: the raw style string is the source of truth and round-trips
 * verbatim. Editors mutate it only through upsertPair/removePairs, which
 * rewrite the one matching segment and leave every other segment — including
 * pairs this version does not recognize — byte-identical.
 */

// ---------------------------------------------------------------------------
// Typed property bags
// ---------------------------------------------------------------------------

export function emptyProps() {
  return {
    background: '',        // '#rrggbb' or ''
    textColor: '',
    separatorColor: '',
    width: null,           // {value, isPercent} or null
    height: null,
    margin: null,
    cornerRadius: null,
    borderWidth: null,
    iconSize: null,
    fontSize: null,
    fontName: '',
    textWeight: 0,         // 0 = not set; else 100..900
    iconPosition: '',      // left|right|top|bottom|center or ''
    textHAlign: '',        // left|center|right or ''
    textVAlign: '',        // top|middle|bottom or ''
    textLayoutSmart: false,
    caption: '',           // window scope only
    title: '',
    credits: '',
    resourceFolder: '',
  };
}

// ---------------------------------------------------------------------------
// Value parsing (skip-on-error: return false, leave props untouched)
// ---------------------------------------------------------------------------

function parseColor(value) {
  return /^#[0-9a-fA-F]{6}$/.test(value) ? value.toLowerCase() : null;
}

function parseSize(value) {
  let digits = value;
  let isPercent = false;
  if (digits.endsWith('%')) {
    isPercent = true;
    digits = digits.slice(0, -1).trim();
  }
  if (!digits || digits.length > 6 || !/^[0-9]+$/.test(digits)) return null;
  return { value: parseInt(digits, 10), isPercent };
}

function parseWeight(value) {
  const v = value.trim().toLowerCase();
  if (v === 'normal') return 400;
  if (v === 'bold') return 700;
  if (!/^[0-9]{3}$/.test(v)) return 0;
  const n = parseInt(v, 10);
  return (n >= 100 && n <= 900) ? n : 0;
}

const ICON_POSITIONS = ['left', 'right', 'top', 'bottom', 'center'];
const H_ALIGNS = ['left', 'center', 'right'];
const V_ALIGNS = ['top', 'middle', 'bottom'];

/** One or two space-separated align keywords, order free; all-or-nothing. */
function parseTextLayout(value, props) {
  const tokens = value.trim().split(/\s+/).filter(Boolean);
  if (tokens.length === 0 || tokens.length > 2) return false;
  let h = '', v = '';
  for (const t of tokens) {
    const k = t.toLowerCase();
    if (H_ALIGNS.includes(k)) h = k;
    else if (V_ALIGNS.includes(k)) v = k;
    else return false;
  }
  if (h) props.textHAlign = h;
  if (v) props.textVAlign = v;
  props.textLayoutSmart = false; // explicit alignment overrides smart
  return true;
}

/**
 * Applies one pair to a props bag. windowScope selects the window property
 * set (separator-color + free-text fields, no geometry) versus the button
 * group set. Returns false when the property doesn't exist in the scope or
 * the value is malformed — the caller just skips the pair (STY-F03).
 */
function applyProp(props, name, value, windowScope) {
  let c;
  switch (name) {
    case 'background': return (c = parseColor(value)) ? (props.background = c, true) : false;
    case 'text-color': return (c = parseColor(value)) ? (props.textColor = c, true) : false;
    case 'font-size': return (c = parseSize(value)) ? (props.fontSize = c, true) : false;
    case 'text-weight': return (c = parseWeight(value)) ? (props.textWeight = c, true) : false;
    case 'font-name': return value ? (props.fontName = value, true) : false;
  }

  if (windowScope) {
    switch (name) {
      case 'separator-color': return (c = parseColor(value)) ? (props.separatorColor = c, true) : false;
      case 'caption': return value ? (props.caption = value, true) : false;
      case 'title': return value ? (props.title = value, true) : false;
      case 'credits': return value ? (props.credits = value, true) : false;
      case 'resource-folder': return value ? (props.resourceFolder = value, true) : false;
    }
    return false;
  }

  switch (name) {
    case 'width': return (c = parseSize(value)) ? (props.width = c, true) : false;
    case 'height': return (c = parseSize(value)) ? (props.height = c, true) : false;
    case 'margin': return (c = parseSize(value)) ? (props.margin = c, true) : false;
    case 'corner-radius': return (c = parseSize(value)) ? (props.cornerRadius = c, true) : false;
    case 'border-width': return (c = parseSize(value)) ? (props.borderWidth = c, true) : false;
    case 'icon-size': return (c = parseSize(value)) ? (props.iconSize = c, true) : false;
    case 'icon-position': {
      const v = value.trim().toLowerCase();
      return ICON_POSITIONS.includes(v) ? (props.iconPosition = v, true) : false;
    }
    case 'text-layout': {
      const v = value.trim().toLowerCase();
      if (v === 'smart' || v === 'auto') {
        props.textLayoutSmart = true;
        props.textHAlign = '';
        props.textVAlign = '';
        return true;
      }
      return parseTextLayout(value, props);
    }
  }
  return false;
}

// ---------------------------------------------------------------------------
// Pair iteration & segment-preserving editing
// ---------------------------------------------------------------------------

/** Calls cb(lowercasedTrimmedName, trimmedValue) per well-formed pair. */
export function forEachPair(list, cb) {
  for (const segment of String(list ?? '').split(';')) {
    const colon = segment.indexOf(':');
    if (colon === -1) continue;
    const name = segment.slice(0, colon).trim().toLowerCase();
    if (!name) continue;
    cb(name, segment.slice(colon + 1).trim());
  }
}

/** Last-wins value of a property in a raw list, or null when unset. */
export function getPair(raw, name) {
  let found = null;
  forEachPair(raw, (n, v) => { if (n === name.toLowerCase()) found = v; });
  return found;
}

/**
 * Returns a new raw list with `name: value` — replacing the LAST segment that
 * parses to that name, else appending. Every other segment (unknown or even
 * malformed) is preserved verbatim; empty segments are dropped.
 */
export function upsertPair(raw, name, value) {
  const segments = String(raw ?? '').split(';').filter((s) => s.trim() !== '');
  const target = name.toLowerCase();
  let lastIdx = -1;
  segments.forEach((segment, i) => {
    const colon = segment.indexOf(':');
    if (colon === -1) return;
    if (segment.slice(0, colon).trim().toLowerCase() === target) lastIdx = i;
  });
  const pair = `${name}:${value}`;
  if (lastIdx >= 0) segments[lastIdx] = pair;
  else segments.push(pair);
  return segments.join(';') + ';';
}

/** Returns a new raw list with every segment named `name` removed. */
export function removePairs(raw, name) {
  const target = name.toLowerCase();
  const segments = String(raw ?? '').split(';').filter((s) => {
    if (s.trim() === '') return false;
    const colon = s.indexOf(':');
    if (colon === -1) return true; // keep malformed segments verbatim
    return s.slice(0, colon).trim().toLowerCase() !== target;
  });
  return segments.length ? segments.join(';') + ';' : '';
}

// ---------------------------------------------------------------------------
// Layer parsing (board / category) & cascade resolution
// ---------------------------------------------------------------------------

/** Board layer: unprefixed = window; category-/phrase- prefixes = groups. */
export function parseBoardStyle(raw) {
  const out = { window: emptyProps(), categoryButtons: emptyProps(), phraseButtons: emptyProps() };
  forEachPair(raw, (name, value) => {
    if (name.startsWith('category-')) applyProp(out.categoryButtons, name.slice(9), value, false);
    else if (name.startsWith('phrase-')) applyProp(out.phraseButtons, name.slice(7), value, false);
    else applyProp(out.window, name, value, true);
  });
  return out;
}

/** Category layer: own colors (background/text-color) + phrase-* overrides. */
export function parseCategoryStyle(raw) {
  const out = { ownButton: emptyProps(), phraseButtons: emptyProps() };
  forEachPair(raw, (name, value) => {
    if (name.startsWith('phrase-')) applyProp(out.phraseButtons, name.slice(7), value, false);
    else if (name === 'background' || name === 'text-color') applyProp(out.ownButton, name, value, false);
    // anything else: skipped (STY-F30)
  });
  return out;
}

/** base with every property set in `over` overridden (per-property merge). */
export function resolveEffective(base, over) {
  const r = { ...base };
  if (over.background) r.background = over.background;
  if (over.textColor) r.textColor = over.textColor;
  if (over.separatorColor) r.separatorColor = over.separatorColor;
  for (const k of ['width', 'height', 'margin', 'cornerRadius', 'borderWidth', 'iconSize', 'fontSize']) {
    if (over[k]) r[k] = over[k];
  }
  if (over.fontName) r.fontName = over.fontName;
  if (over.textWeight) r.textWeight = over.textWeight;
  if (over.iconPosition) r.iconPosition = over.iconPosition;
  // text-layout resolves atomically: smart resets inherited axes; an explicit
  // axis overrides that axis and drops the inherited smart flag.
  if (over.textLayoutSmart) {
    r.textLayoutSmart = true;
    r.textHAlign = '';
    r.textVAlign = '';
  }
  if (over.textHAlign) { r.textHAlign = over.textHAlign; r.textLayoutSmart = false; }
  if (over.textVAlign) { r.textVAlign = over.textVAlign; r.textLayoutSmart = false; }
  for (const k of ['caption', 'title', 'credits', 'resourceFolder']) {
    if (over[k]) r[k] = over[k];
  }
  return r;
}

/**
 * Size against its built-in default: unset → default; percent → % of default;
 * absolute → pixels. Zero honored only where meaningful (allowZero).
 */
export function resolveSize(size, defaultValue, allowZero = false) {
  if (!size) return defaultValue;
  const value = size.isPercent ? Math.floor((defaultValue * size.value) / 100) : size.value;
  if (value < 0 || (value === 0 && !allowZero)) return defaultValue;
  return value;
}

/** Black or white, whichever contrasts with the background (Rec. 601 luma). */
export function contrastColor(hex) {
  const n = parseInt(hex.slice(1), 16);
  const lum = (299 * ((n >> 16) & 0xff) + 587 * ((n >> 8) & 0xff) + 114 * (n & 0xff)) / 1000;
  return lum < 128 ? '#ffffff' : '#000000';
}

/** Preview-only shade derivation (approximates the app's hover bevels). */
export function shadeColor(hex, factor) {
  const n = parseInt(hex.slice(1), 16);
  const ch = (v) => Math.max(0, Math.min(255, Math.round(v * factor)));
  const r = ch((n >> 16) & 0xff), g = ch((n >> 8) & 0xff), b = ch(n & 0xff);
  return '#' + ((r << 16) | (g << 8) | b).toString(16).padStart(6, '0');
}
