/**
 * arasaac.js — direct-from-browser client for the official ARASAAC API
 * (https://arasaac.org/developers). CORS verified: both hosts respond with
 * Access-Control-Allow-Origin: *. No server proxy is involved; the only data
 * sent is the search keyword and locale.
 *
 * ARASAAC pictograms are CC BY-NC-SA — owner: Government of Aragón, author:
 * Sergio Palao. Attribution is mandatory wherever they are used; callers must
 * surface ATTRIBUTION_TEXT in the picker UI and append it to the board's
 * `credits` (never containing ';' or '|', which are style-carrier reserved).
 */

const API_HOST = 'https://api.arasaac.org';
const STATIC_HOST = 'https://static.arasaac.org';

export const ATTRIBUTION_TEXT =
  'Pictograms: Sergio Palao / ARASAAC (CC BY-NC-SA) - Government of Aragon';

export const ARASAAC_LOCALES = [
  'an', 'ar', 'bg', 'br', 'ca', 'cs', 'da', 'de', 'el', 'en', 'es', 'et', 'eu',
  'fa', 'fr', 'gl', 'he', 'hr', 'hu', 'it', 'ko', 'lt', 'lv', 'mk', 'nl', 'pl',
  'pt', 'ro', 'ru', 'sk', 'sq', 'sv', 'sr', 'val', 'uk', 'zh',
];

const CACHE_PREFIX = 'ssbe-ara:';
const CACHE_TTL_MS = 15 * 60 * 1000;

function cacheGet(key) {
  try {
    const raw = sessionStorage.getItem(CACHE_PREFIX + key);
    if (!raw) return null;
    const { t, data } = JSON.parse(raw);
    if (Date.now() - t > CACHE_TTL_MS) return null;
    return data;
  } catch {
    return null;
  }
}

function cachePut(key, data) {
  try {
    sessionStorage.setItem(CACHE_PREFIX + key, JSON.stringify({ t: Date.now(), data }));
  } catch {
    /* storage full/blocked: caching is best-effort */
  }
}

/** Thumbnail URL for a result (hotlinked for the picker grid only). */
export function thumbnailUrl(id) {
  return `${STATIC_HOST}/pictograms/${encodeURIComponent(id)}/${encodeURIComponent(id)}_300.png`;
}

/**
 * Searches pictograms. Returns [{id, keyword}] (deduplicated, capped).
 * Throws Error('search-failed') on network/API errors.
 */
export async function searchPictograms(locale, term, limit = 40) {
  const q = term.trim();
  if (!q) return [];
  const loc = ARASAAC_LOCALES.includes(locale) ? locale : 'en';
  const key = `${loc}:${q.toLowerCase()}`;
  const cached = cacheGet(key);
  if (cached) return cached;

  let res;
  try {
    res = await fetch(`${API_HOST}/api/pictograms/${loc}/search/${encodeURIComponent(q)}`, {
      headers: { Accept: 'application/json' },
    });
  } catch {
    throw new Error('search-failed');
  }
  if (res.status === 404) {
    cachePut(key, []);
    return []; // ARASAAC answers 404 for "no results"
  }
  if (!res.ok) throw new Error('search-failed');

  const json = await res.json();
  const out = [];
  const seen = new Set();
  for (const item of Array.isArray(json) ? json : []) {
    const id = item?._id;
    if (typeof id !== 'number' || seen.has(id)) continue;
    seen.add(id);
    out.push({ id, keyword: item?.keywords?.[0]?.keyword ?? String(id) });
    if (out.length >= limit) break;
  }
  cachePut(key, out);
  return out;
}

/**
 * Downloads the 300px PNG for a pictogram. Returns { bytes }.
 * Throws Error('download-failed') / Error('too-large').
 */
export async function downloadPictogram(id, maxBytes) {
  let res;
  try {
    res = await fetch(thumbnailUrl(id));
  } catch {
    throw new Error('download-failed');
  }
  if (!res.ok) throw new Error('download-failed');
  const bytes = new Uint8Array(await res.arrayBuffer());
  if (maxBytes && bytes.length > maxBytes) throw new Error('too-large');
  return { bytes };
}

/**
 * Default resource basename for a downloaded pictogram: the user's search
 * text, sanitized to a safe flat name (format markers, path characters and
 * control chars stripped; spaces → '_'; trimmed; capped). Empty result →
 * caller falls back to `arasaac_<id>`.
 */
export function sanitizePictogramName(searchText) {
  let name = '';
  for (const ch of String(searchText ?? '')) {
    const c = ch.codePointAt(0);
    if (c < 0x20) continue;
    if ('\\/:*?"<>|=;♫'.includes(ch)) continue;
    name += ch === ' ' ? '_' : ch;
  }
  name = name.replace(/#/g, '').replace(/\.+/g, '_'); // no ## marker, no dots (extension is appended)
  name = name.replace(/^_+|_+$/g, '');
  if (name.length > 40) name = name.slice(0, 40).replace(/_+$/g, '');
  return name;
}

/** First free name among `base.png`, `base_2.png`, … given taken basenames. */
export function uniquePictogramName(base, taken) {
  let candidate = `${base}.png`;
  for (let i = 2; taken.has(candidate); i++) candidate = `${base}_${i}.png`;
  return candidate;
}
