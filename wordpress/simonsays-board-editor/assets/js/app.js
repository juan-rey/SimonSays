/**
 * app.js — editor orchestrator. Stateless by design: boards live only in this
 * browser tab (plus an IndexedDB autosave for crash/refresh recovery) and
 * leave it exclusively as downloads. The only network traffic is the ARASAAC
 * pictogram search/download, called directly from the browser.
 *
 * Bootstrapped by the WordPress embed, which provides window.SSBE_DATA:
 *   { strings, settings: { maxResourceBytes, maxTotalBytes,
 *     arasaac: { enabled, locale, attribution }, defaultTemplate },
 *     templates: [{id, name, file}], baseUrl, languages: [{code, name, rtl}] }
 */

import { parseSsc, serializeSsc, sscFileBytes, referencedResources, SscError } from './ssc.js';
import { parseSsz, buildSsz, isZipBytes, SszError } from './ssz.js';
import { sniff, kindMatchesExt, extOf, isAllowedResourceName } from './sniff.js';
import { validateBoard, hasBlockingErrors } from './validate.js';
import { parseBoardStyle, upsertPair, getPair } from './style.js';
import { renderPreview, createResourceUrlProvider } from './preview.js';
import {
  searchPictograms, downloadPictogram, thumbnailUrl, ARASAAC_LOCALES,
  ATTRIBUTION_TEXT, sanitizePictogramName, uniquePictogramName,
} from './arasaac.js';
import {
  el, buildCategoriesPanel, buildPhrasesPanel, buildBoardStylePanel,
  buildCategoryStylePanel, buildResourcesPanel, buildArasaacPanel, buildFindingsPanel,
} from './ui.js';

// ---------------------------------------------------------------------------
// IndexedDB autosave (client-side only; cleared by "New" and after recovery)
// ---------------------------------------------------------------------------

function idb() {
  return new Promise((resolve, reject) => {
    const req = indexedDB.open('ssbe-editor', 1);
    req.onupgradeneeded = () => req.result.createObjectStore('autosave');
    req.onsuccess = () => resolve(req.result);
    req.onerror = () => reject(req.error);
  });
}

async function idbPut(value) {
  const db = await idb();
  await new Promise((resolve, reject) => {
    const tx = db.transaction('autosave', 'readwrite');
    tx.objectStore('autosave').put(value, 'current');
    tx.oncomplete = resolve;
    tx.onerror = () => reject(tx.error);
  });
  db.close();
}

async function idbGet() {
  const db = await idb();
  const value = await new Promise((resolve, reject) => {
    const req = db.transaction('autosave', 'readonly').objectStore('autosave').get('current');
    req.onsuccess = () => resolve(req.result);
    req.onerror = () => reject(req.error);
  });
  db.close();
  return value;
}

async function idbClear() {
  const db = await idb();
  await new Promise((resolve, reject) => {
    const tx = db.transaction('autosave', 'readwrite');
    tx.objectStore('autosave').delete('current');
    tx.oncomplete = resolve;
    tx.onerror = () => reject(tx.error);
  });
  db.close();
}

// ---------------------------------------------------------------------------

function emptyBoard() {
  return { boardStyle: '', categories: [] };
}

export async function initEditor(root, data) {
  const t = (key) => data.strings[key] ?? key;

  const state = {
    board: emptyBoard(),
    resources: new Map(), // basename -> Uint8Array
    selectedCategory: 0,
    language: (data.languages[0] && data.languages[0].code) || 'en',
    dirty: false,
    openPanels: {},       // panel key -> open?; seeded by each panel's default
    arasaacTarget: null,  // { kind: 'category'|'phrase', catIndex, phraseIndex? }
    ara: { term: '', locale: data.settings.arasaac.locale || 'en', results: null },
    highlightPhrase: null,
  };
  const urls = createResourceUrlProvider();
  let audio = null;
  let autosaveTimer = 0;
  let scrollTo = null; // { panel } | { phrase } — consumed after the next refresh

  // Board language code -> Web Speech API language tag.
  const speechLang = (code) => (code === 'val' ? 'ca' : code === 'zh' ? 'zh-CN' : code);

  function speak(text) {
    if (!('speechSynthesis' in window)) {
      ctx.showError(t('err-speech-unavailable'));
      return;
    }
    window.speechSynthesis.cancel();
    const utter = new SpeechSynthesisUtterance(text);
    utter.lang = speechLang(state.language);
    window.speechSynthesis.speak(utter);
  }

  // --- Skeleton ------------------------------------------------------------
  root.classList.add('ssbe-root');
  const banner = el('div', { class: 'ssbe-banner', role: 'alert', 'aria-live': 'assertive', hidden: 'hidden' });
  const toolbar = el('div', { class: 'ssbe-toolbar', role: 'toolbar', 'aria-label': t('toolbar-label') });
  const left = el('div', { class: 'ssbe-left' });
  const right = el('div', { class: 'ssbe-right' });
  const previewBox = el('div', { class: 'ssbe-preview-box' }); // persistent node, reused across refreshes
  root.append(banner, toolbar, el('div', { class: 'ssbe-columns' }, [left, right]));

  let findings = [];
  let bannerTimer = 0;

  const ctx = {
    state, data, t, urls,
    showError(message) {
      banner.textContent = message;
      banner.hidden = false;
      clearTimeout(bannerTimer);
      bannerTimer = setTimeout(() => { banner.hidden = true; }, 8000);
    },
    formatKb(bytes) {
      return (bytes / 1024).toFixed(1) + ' KB';
    },
    selectedCategory() {
      return state.board.categories[state.selectedCategory] ?? null;
    },
    referencedNames() {
      return referencedResources(state.board);
    },
    arasaacLocales: () => ARASAAC_LOCALES,
    arasaacThumb: thumbnailUrl,
    searchArasaac: (locale, term) => searchPictograms(locale, term),
    /** Arms assign mode for a specific icon field and opens the panel. */
    armArasaacTarget(target) {
      state.arasaacTarget = target;
      state.openPanels['arasaac'] = true;
      scrollTo = { panel: 'arasaac' };
      ctx.refresh();
    },
    async pickArasaac(item) {
      try {
        const { bytes } = await downloadPictogram(item.id, data.settings.maxResourceBytes);
        // Default name = the search text (sanitized), else the pictogram id.
        const base = sanitizePictogramName(state.ara.term) || `arasaac_${item.id}`;
        const name = uniquePictogramName(base, new Set(state.resources.keys()));
        state.resources.set(name, bytes);
        appendArasaacCredit();

        // Assign to the armed target field when it still exists.
        const target = state.arasaacTarget;
        if (target) {
          const cat = state.board.categories[target.catIndex];
          if (target.kind === 'category' && cat) cat.icon = name;
          else if (target.kind === 'phrase' && cat && cat.phrases[target.phraseIndex]) {
            cat.phrases[target.phraseIndex].icon = name;
          }
          state.arasaacTarget = null;
        }
        ctx.showError(t('arasaac-added') + ' ' + name);
        ctx.onBoardChanged();
      } catch (e) {
        ctx.showError(t(e.message === 'too-large' ? 'err-resource-too-large' : 'arasaac-error'));
      }
    },
    async addResourceFile(file) {
      const name = file.name;
      if (!isAllowedResourceName(name)) {
        ctx.showError(t('err-resource-bad-extension') + ' — ' + name);
        return;
      }
      if (file.size > data.settings.maxResourceBytes) {
        ctx.showError(t('err-resource-too-large') + ' — ' + name);
        return;
      }
      const bytes = new Uint8Array(await file.arrayBuffer());
      if (!kindMatchesExt(sniff(bytes), extOf(name))) {
        ctx.showError(t('err-content-mismatch') + ' — ' + name);
        return;
      }
      if (state.resources.has(name) && !window.confirm(t('confirm-replace-resource') + ' ' + name)) return;
      state.resources.set(name, bytes);
      ctx.onBoardChanged();
    },
    onBoardChanged() {
      state.dirty = true;
      scheduleAutosave();
      ctx.refresh();
    },
    refresh,
  };

  function appendArasaacCredit() {
    const credits = getPair(state.board.boardStyle, 'credits') ?? '';
    if (!credits.includes('ARASAAC')) {
      const attribution = data.settings.arasaac.attribution || ATTRIBUTION_TEXT;
      const value = credits ? credits + ' - ' + attribution : attribution;
      state.board.boardStyle = upsertPair(state.board.boardStyle, 'credits', value);
    }
  }

  // --- Toolbar -------------------------------------------------------------
  const templateSelect = el('select', { class: 'ssbe-input', 'aria-label': t('template-label') },
    data.templates.map((tpl) => el('option', { value: tpl.id, text: tpl.name })));
  if (data.settings.defaultTemplate) templateSelect.value = data.settings.defaultTemplate;

  const languageSelect = el('select', {
    class: 'ssbe-input', 'aria-label': t('language-label'),
    onchange: (e) => { state.language = e.target.value; ctx.onBoardChanged(); },
  }, data.languages.map((l) => el('option', { value: l.code, text: l.name })));
  languageSelect.value = state.language;

  const openInput = el('input', {
    type: 'file', accept: '.ssc,.ssz', class: 'ssbe-hidden-file',
    onchange: async (e) => {
      const file = e.target.files[0];
      e.target.value = '';
      if (file) await openFile(file);
    },
  });

  toolbar.append(
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: t('tb-new'), onclick: () => newBoard(templateSelect.value) }),
    templateSelect,
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: t('tb-open'), onclick: () => openInput.click() }),
    openInput,
    el('button', { type: 'button', class: 'ssbe-btn-primary', id: 'ssbe-export-auto', text: t('tb-export'), onclick: () => exportBoard(null) }),
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: t('tb-export-ssc'), onclick: () => exportBoard('ssc') }),
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: t('tb-export-ssz'), onclick: () => exportBoard('ssz') }),
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: t('tb-validate'), onclick: () => { runValidation(); ctx.refresh(); } }),
    languageSelect,
  );

  // --- Board lifecycle -----------------------------------------------------
  async function newBoard(templateId) {
    if (state.dirty && !window.confirm(t('confirm-discard'))) return;
    const tpl = data.templates.find((x) => x.id === templateId);
    let resources = new Map();
    try {
      if (tpl && tpl.url) {
        const res = await fetch(tpl.url);
        if (!res.ok) throw new Error('fetch');
        const bytes = new Uint8Array(await res.arrayBuffer());
        if (isZipBytes(bytes)) {
          // Admin templates may be .ssz bundles: same validation as any import.
          const parsed = await parseSsz(bytes, {
            maxResourceBytes: data.settings.maxResourceBytes,
            maxTotalBytes: data.settings.maxTotalBytes,
          });
          state.board = parseSsc(parsed.sscText);
          resources = parsed.resources;
        } else {
          state.board = parseSsc(new TextDecoder('utf-8').decode(bytes));
        }
        if (tpl.language) {
          state.language = tpl.language;
          languageSelect.value = tpl.language;
        }
      } else {
        state.board = emptyBoard();
      }
    } catch {
      ctx.showError(t('err-template-load'));
      state.board = emptyBoard();
    }
    state.resources = resources;
    urls.revokeAll();
    state.selectedCategory = 0;
    state.dirty = false;
    state.arasaacTarget = null;
    state.highlightPhrase = null;
    findings = [];
    await idbClear().catch(() => {});
    ctx.refresh();
  }

  async function openFile(file) {
    if (state.dirty && !window.confirm(t('confirm-discard'))) return;
    try {
      const bytes = new Uint8Array(await file.arrayBuffer());
      let board;
      let resources = new Map();
      if (isZipBytes(bytes) || file.name.toLowerCase().endsWith('.ssz')) {
        const parsed = await parseSsz(bytes, {
          maxResourceBytes: data.settings.maxResourceBytes,
          maxTotalBytes: data.settings.maxTotalBytes,
        });
        board = parseSsc(parsed.sscText);
        resources = parsed.resources;
      } else {
        board = parseSsc(new TextDecoder('utf-8').decode(bytes));
      }
      state.board = board;
      state.resources = resources;
      urls.revokeAll();
      state.selectedCategory = 0;
      state.dirty = false;
      findings = [];
      ctx.refresh();
    } catch (e) {
      if (e instanceof SszError) ctx.showError(t('err-ssz-' + e.code) || (t('err-import') + ' (' + e.code + ')'));
      else if (e instanceof SscError) ctx.showError(t('err-ssc-' + e.code) || t('err-import'));
      else ctx.showError(t('err-import'));
    }
  }

  function suggestedFileName(ext) {
    const now = new Date();
    const stamp = now.getFullYear() * 10000 + (now.getMonth() + 1) * 100 + now.getDate();
    const styles = parseBoardStyle(state.board.boardStyle);
    let title = styles.window.title || 'board';
    title = title.replace(/ /g, '_').replace(/[\\/:*?"<>|]/g, '');
    return `${stamp}_${title || 'board'}_${state.language}.${ext}`;
  }

  function download(bytes, filename) {
    const url = URL.createObjectURL(new Blob([bytes], { type: 'application/octet-stream' }));
    const a = el('a', { href: url, download: filename });
    document.body.appendChild(a);
    a.click();
    a.remove();
    setTimeout(() => URL.revokeObjectURL(url), 5000);
  }

  function runValidation() {
    findings = validateBoard(state.board, state.resources, {
      maxResourceBytes: data.settings.maxResourceBytes,
      maxTotalBytes: data.settings.maxTotalBytes,
    });
    return findings;
  }

  function exportBoard(forcedExt) {
    runValidation();
    if (hasBlockingErrors(findings)) {
      ctx.showError(t('err-fix-before-export'));
      ctx.refresh();
      return;
    }
    const text = serializeSsc(state.board);
    const referenced = referencedResources(state.board);
    const bundledCount = [...referenced].filter((n) => state.resources.has(n)).length;
    const ext = forcedExt ?? (bundledCount > 0 ? 'ssz' : 'ssc');
    try {
      if (ext === 'ssz') {
        download(buildSsz(text, state.resources, referenced, { maxTotalBytes: data.settings.maxTotalBytes }), suggestedFileName('ssz'));
      } else {
        download(sscFileBytes(text), suggestedFileName('ssc'));
      }
      state.dirty = false;
    } catch (e) {
      ctx.showError(e instanceof SszError ? (t('err-ssz-' + e.code) || t('err-export')) : t('err-export'));
    }
  }

  // --- Autosave / recovery -------------------------------------------------
  function scheduleAutosave() {
    clearTimeout(autosaveTimer);
    autosaveTimer = setTimeout(() => {
      idbPut({
        sscText: serializeSsc(state.board),
        language: state.language,
        resources: [...state.resources.entries()],
        time: Date.now(),
      }).catch(() => {});
    }, 1000);
  }

  async function offerRecovery() {
    let saved = null;
    try { saved = await idbGet(); } catch { /* no IndexedDB: skip */ }
    if (!saved || !saved.sscText) return false;
    if (!window.confirm(t('confirm-recover'))) {
      await idbClear().catch(() => {});
      return false;
    }
    try {
      state.board = parseSsc(saved.sscText);
      state.language = saved.language || state.language;
      languageSelect.value = state.language;
      state.resources = new Map(saved.resources || []);
      state.dirty = true;
      return true;
    } catch {
      await idbClear().catch(() => {});
      return false;
    }
  }

  window.addEventListener('beforeunload', (e) => {
    if (state.dirty) {
      e.preventDefault();
      e.returnValue = '';
    }
  });

  // --- Rendering -----------------------------------------------------------
  function playResource(name) {
    const url = urls.urlFor(state.resources, name);
    if (!url) {
      ctx.showError(t('err-audio-missing') + ' — ' + name);
      return;
    }
    if (audio) audio.pause();
    audio = new Audio(url);
    audio.play().catch(() => ctx.showError(t('err-audio-play')));
  }

  function refresh() {
    const langInfo = data.languages.find((l) => l.code === state.language);
    const rtl = !!(langInfo && langInfo.rtl);

    // Left: content editing. Right: board style, preview, resources, findings.
    left.replaceChildren(
      buildCategoriesPanel(ctx),
      buildCategoryStylePanel(ctx),
      buildPhrasesPanel(ctx),
      buildArasaacPanel(ctx),
    );
    right.replaceChildren(
      buildBoardStylePanel(ctx),
      el('h3', { text: t('panel-preview') }),
      previewBox,
      buildResourcesPanel(ctx),
      buildFindingsPanel(ctx, findings),
    );

    renderPreview(previewBox, {
      board: state.board,
      resourceUrl: (name) => urls.urlFor(state.resources, name),
      selectedIndex: state.selectedCategory,
      rtl,
      hintText: t('preview-hint'),
      onSelectCategory: (i) => {
        state.selectedCategory = i;
        state.highlightPhrase = null;
        refresh();
      },
      onPhraseClick: (i, phrase) => {
        // Highlight the phrase row in the (opened) Phrases panel, then speak
        // the text — or play its sound file when it has one.
        state.highlightPhrase = i;
        state.openPanels['phrases'] = true;
        scrollTo = { phrase: i };
        refresh();
        if (phrase.audioFile) playResource(phrase.audioFile);
        else speak(phrase.text);
      },
    });

    const exportAuto = toolbar.querySelector('#ssbe-export-auto');
    const referenced = referencedResources(state.board);
    const hasBundled = [...referenced].some((n) => state.resources.has(n));
    exportAuto.textContent = hasBundled ? t('tb-export') + ' (.ssz)' : t('tb-export') + ' (.ssc)';

    // Deferred scrolls requested by this refresh cycle.
    if (scrollTo) {
      const want = scrollTo;
      scrollTo = null;
      requestAnimationFrame(() => {
        let node = null;
        if (want.panel) node = root.querySelector(`[data-panel="${want.panel}"]`);
        else if (want.phrase !== undefined) node = root.querySelector(`[data-phrase-index="${want.phrase}"]`);
        node?.scrollIntoView({ block: 'nearest', behavior: 'smooth' });
      });
    }
  }

  // --- Boot ----------------------------------------------------------------
  const recovered = await offerRecovery();
  if (!recovered) await newBoard(templateSelect.value);
  else refresh();
}

// Auto-boot every embed placeholder on the page (browser only — the module
// is also importable under Node for tests).
if (typeof document !== 'undefined') {
  for (const mount of document.querySelectorAll('.ssbe-mount')) {
    if (window.SSBE_DATA) initEditor(mount, window.SSBE_DATA);
  }
}
