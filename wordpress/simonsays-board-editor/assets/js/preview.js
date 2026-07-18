/**
 * preview.js — live, client-side approximation of the SimonSays category
 * window: cascade-resolved colors/geometry/fonts, category grid + selected
 * category's phrase grid + caption strip, emoji/image icons, derived hover
 * shades, RTL mirroring. Board-derived text always lands in text nodes —
 * never innerHTML — so imported content cannot inject markup.
 */

import {
  parseBoardStyle, parseCategoryStyle, resolveEffective, resolveSize,
  contrastColor, shadeColor, emptyProps,
} from './style.js';
import { mimeForExt, extOf } from './sniff.js';

// Built-in defaults mirroring the desktop app's category window.
const DEFAULTS = {
  buttonWidth: 120,
  buttonHeight: 40,
  margin: 10,
  windowBg: '#f3f3f3',
  buttonBg: '#fdfdfd',
  fontPx: 15,
  cornerRadius: 4,
  borderWidth: 1,
};

function el(tag, className, text) {
  const n = document.createElement(tag);
  if (className) n.className = className;
  if (text !== undefined) n.textContent = text;
  return n;
}

/** Applies resolved button props as inline styles + CSS custom properties. */
function styleButton(btn, props, resourceUrl) {
  const bg = props.background || DEFAULTS.buttonBg;
  const fg = props.textColor || contrastColor(bg);
  const w = resolveSize(props.width, DEFAULTS.buttonWidth);
  const h = resolveSize(props.height, DEFAULTS.buttonHeight);
  const radius = resolveSize(props.cornerRadius, DEFAULTS.cornerRadius, true);
  const border = Math.min(2, resolveSize(props.borderWidth, DEFAULTS.borderWidth, true));
  const fontPx = resolveSize(props.fontSize, DEFAULTS.fontPx);

  btn.style.setProperty('--ssbe-bg', bg);
  btn.style.setProperty('--ssbe-bg-hover', shadeColor(bg, 0.92));
  btn.style.setProperty('--ssbe-bg-active', shadeColor(bg, 0.85));
  btn.style.color = fg;
  btn.style.width = w + 'px';
  btn.style.minHeight = h + 'px';
  btn.style.borderRadius = radius + 'px';
  btn.style.borderWidth = border + 'px';
  btn.style.borderColor = shadeColor(bg, 0.75);
  btn.style.fontSize = fontPx + 'px';
  if (props.fontName) btn.style.fontFamily = props.fontName;
  if (props.textWeight) btn.style.fontWeight = String(props.textWeight);
  return { w, h };
}

/**
 * Lays out icon + label inside a button per icon-position / text-layout.
 * Smart default (no explicit alignment): label centered in the leftover area.
 */
function fillButton(btn, { icon, label, resourceUrl, props, iconAlt }) {
  btn.replaceChildren();
  const iconPos = props.iconPosition || 'left';
  const explicit = !!(props.textHAlign || props.textVAlign);

  let iconNode = null;
  if (icon) {
    if (icon.includes('.')) {
      const url = resourceUrl(icon);
      if (url) {
        iconNode = document.createElement('img');
        iconNode.src = url;
        iconNode.alt = iconAlt || '';
        iconNode.className = 'ssbe-btn-icon-img';
      } else {
        iconNode = el('span', 'ssbe-btn-icon-missing', '⌧'); // dangling reference
      }
      const size = resolveSize(props.iconSize, 0, true);
      if (iconNode.tagName === 'IMG' && size > 0) {
        iconNode.style.width = size + 'px';
        iconNode.style.height = size + 'px';
      }
    } else {
      iconNode = el('span', 'ssbe-btn-icon-emoji', icon);
      const size = resolveSize(props.iconSize, 0, true);
      if (size > 0) iconNode.style.fontSize = size + 'px';
    }
  }

  const labelNode = el('span', 'ssbe-btn-label', label);
  if (props.textHAlign) labelNode.style.textAlign = props.textHAlign;

  // icon-position:center → icon only, unless an explicit text-layout keeps
  // the label (drawn over/after the centered icon, best-effort approximation).
  if (iconNode && iconPos === 'center' && !explicit) {
    btn.classList.add('ssbe-btn-center-icon');
    btn.appendChild(iconNode);
    return;
  }

  btn.classList.add('ssbe-btn-pos-' + (iconNode ? iconPos : 'none'));
  if (props.textVAlign) btn.classList.add('ssbe-btn-v-' + props.textVAlign);
  if (props.textHAlign) btn.classList.add('ssbe-btn-h-' + props.textHAlign);

  if (iconNode && (iconPos === 'left' || iconPos === 'top' || iconPos === 'center')) btn.appendChild(iconNode);
  btn.appendChild(labelNode);
  if (iconNode && (iconPos === 'right' || iconPos === 'bottom')) btn.appendChild(iconNode);
}

/** STY-F12: category own colors become the base for its phrase buttons. */
function flowThroughColors(own) {
  const flow = emptyProps();
  if (own.background) flow.background = own.background;
  if (own.textColor) flow.textColor = own.textColor;
  return flow;
}

/**
 * Renders the preview into `container`.
 * opts: { board, resourceUrl(name) -> objectURL|null, selectedIndex, rtl,
 *         hintText, onSelectCategory(i), onPhraseClick(index, phrase) }
 */
export function renderPreview(container, opts) {
  const { board, resourceUrl, rtl } = opts;
  const styles = parseBoardStyle(board.boardStyle);

  container.replaceChildren();
  container.classList.add('ssbe-preview');
  container.dir = rtl ? 'rtl' : 'ltr';

  const windowBg = styles.window.background || DEFAULTS.windowBg;
  const windowFg = styles.window.textColor || contrastColor(windowBg);
  container.style.background = windowBg;
  container.style.color = windowFg;

  const selected = Math.min(Math.max(0, opts.selectedIndex ?? 0), Math.max(0, board.categories.length - 1));

  // --- Category grid ------------------------------------------------------
  const catGrid = el('div', 'ssbe-preview-grid ssbe-preview-cats');
  const margin = resolveSize(styles.categoryButtons.margin, DEFAULTS.margin, true);
  catGrid.style.gap = margin + 'px';
  catGrid.style.padding = margin + 'px';

  board.categories.forEach((cat, i) => {
    const catStyle = parseCategoryStyle(cat.styleRaw);
    const props = resolveEffective(styles.categoryButtons, catStyle.ownButton);
    const btn = el('button', 'ssbe-btn');
    btn.type = 'button';
    styleButton(btn, props, resourceUrl);
    fillButton(btn, { icon: cat.icon, label: cat.name, resourceUrl, props });
    if (i === selected) {
      btn.classList.add('ssbe-btn-selected'); // bold + flat marker, like the app
    }
    btn.addEventListener('click', () => opts.onSelectCategory?.(i));
    catGrid.appendChild(btn);
  });
  container.appendChild(catGrid);

  // --- Caption strip between the separators -------------------------------
  const strip = el('div', 'ssbe-preview-strip');
  const sepColor = styles.window.separatorColor || shadeColor(windowBg, 0.8);
  const mkSep = () => {
    const s = el('span', 'ssbe-preview-sep');
    s.style.background = sepColor;
    return s;
  };
  const stripText = el('span', 'ssbe-preview-caption', styles.window.caption || opts.hintText || '');
  if (styles.window.fontName) stripText.style.fontFamily = styles.window.fontName;
  if (styles.window.textWeight) stripText.style.fontWeight = String(styles.window.textWeight);
  if (styles.window.fontSize) stripText.style.fontSize = resolveSize(styles.window.fontSize, DEFAULTS.fontPx) + 'px';
  strip.append(mkSep(), stripText, mkSep());
  container.appendChild(strip);

  // --- Phrase grid for the selected category ------------------------------
  const cat = board.categories[selected];
  if (cat) {
    const catStyle = parseCategoryStyle(cat.styleRaw);
    // Cascade: board phrase defaults → own-color flow-through → phrase-* overrides.
    let phraseProps = resolveEffective(styles.phraseButtons, flowThroughColors(catStyle.ownButton));
    phraseProps = resolveEffective(phraseProps, catStyle.phraseButtons);

    const grid = el('div', 'ssbe-preview-grid ssbe-preview-phrases');
    const pMargin = resolveSize(phraseProps.margin, DEFAULTS.margin, true);
    grid.style.gap = pMargin + 'px';
    grid.style.padding = pMargin + 'px';

    cat.phrases.forEach((p, i) => {
      const btn = el('button', 'ssbe-btn');
      btn.type = 'button';
      styleButton(btn, phraseProps, resourceUrl);
      const label = p.audioFile ? `♫${p.text}♫` : p.text;
      fillButton(btn, { icon: p.icon, label, resourceUrl, props: phraseProps });
      if (p.audioFile) {
        btn.classList.add('ssbe-btn-audio');
        btn.title = p.audioFile;
      }
      // Every phrase key is live, like in the app: highlight + speak/play.
      btn.addEventListener('click', () => opts.onPhraseClick?.(i, p));
      grid.appendChild(btn);
    });
    container.appendChild(grid);
  }
}

/** Builds (and caches) object URLs for the board's binary resources. */
export function createResourceUrlProvider() {
  const urls = new Map(); // name -> {bytes, url}
  return {
    urlFor(resources, name) {
      const bytes = resources.get(name);
      if (!bytes) {
        this.revoke(name);
        return null;
      }
      const cached = urls.get(name);
      if (cached && cached.bytes === bytes) return cached.url;
      if (cached) URL.revokeObjectURL(cached.url);
      const url = URL.createObjectURL(new Blob([bytes], { type: mimeForExt(extOf(name)) }));
      urls.set(name, { bytes, url });
      return url;
    },
    revoke(name) {
      const cached = urls.get(name);
      if (cached) {
        URL.revokeObjectURL(cached.url);
        urls.delete(name);
      }
    },
    revokeAll() {
      for (const { url } of urls.values()) URL.revokeObjectURL(url);
      urls.clear();
    },
  };
}
