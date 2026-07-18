/**
 * ui.js — editor panels. Vanilla DOM, no dependencies. All board-derived
 * content is inserted via textContent/value (never innerHTML). The style
 * forms are generated from a declarative schema and read/write the raw style
 * string exclusively through getPair/upsertPair/removePairs, so pairs this
 * editor does not know survive verbatim (round-trip guarantee).
 *
 * Panels rebuild on every refresh; their collapsed/expanded state persists in
 * ctx.state.openPanels (key -> bool), seeded by each panel's default.
 */

import { getPair, upsertPair, removePairs } from './style.js';
import { categoryNameError, phraseTextError, iconTokenError, styleFreeTextError, renameResourceReferences } from './ssc.js';
import { isAllowedResourceName, extOf } from './sniff.js';

export function el(tag, attrs = {}, children = []) {
  const n = document.createElement(tag);
  for (const [k, v] of Object.entries(attrs)) {
    if (k === 'class') n.className = v;
    else if (k === 'text') n.textContent = v;
    else if (k === 'dataset') Object.assign(n.dataset, v);
    else if (k.startsWith('on')) n.addEventListener(k.slice(2), v);
    else if (v !== undefined && v !== null) n.setAttribute(k, v);
  }
  for (const c of [].concat(children)) if (c) n.appendChild(c);
  return n;
}

let idSeq = 0;
function labelled(labelText, control) {
  const id = 'ssbe-f' + (++idSeq);
  control.id = id;
  return el('div', { class: 'ssbe-field' }, [
    el('label', { for: id, text: labelText }),
    control,
  ]);
}

/** Collapsible panel with persisted open state (survives re-renders). */
function collapsiblePanel(ctx, key, title, children, defaultOpen) {
  const open = ctx.state.openPanels[key] ?? defaultOpen;
  const details = el('details', { class: 'ssbe-panel', dataset: { panel: key } }, [
    el('summary', { text: title }),
    ...children,
  ]);
  if (open) details.setAttribute('open', 'open');
  details.addEventListener('toggle', () => { ctx.state.openPanels[key] = details.open; });
  return details;
}

// ---------------------------------------------------------------------------
// Style forms (declarative)
// ---------------------------------------------------------------------------

const GROUP_PROPS = [
  { prop: 'background', type: 'color' },
  { prop: 'text-color', type: 'color' },
  { prop: 'width', type: 'size' },
  { prop: 'height', type: 'size' },
  { prop: 'margin', type: 'size' },
  { prop: 'corner-radius', type: 'size' },
  { prop: 'border-width', type: 'size' },
  { prop: 'icon-position', type: 'select', options: ['', 'left', 'right', 'top', 'bottom', 'center'] },
  { prop: 'icon-size', type: 'size' },
  { prop: 'font-name', type: 'text' },
  { prop: 'font-size', type: 'size' },
  { prop: 'text-weight', type: 'text', placeholder: 'normal | bold | 100-900' },
  { prop: 'text-layout', type: 'text', placeholder: 'smart | left top …' },
];

const WINDOW_PROPS = [
  { prop: 'background', type: 'color' },
  { prop: 'separator-color', type: 'color' },
  { prop: 'text-color', type: 'color' },
  { prop: 'font-name', type: 'text' },
  { prop: 'font-size', type: 'size' },
  { prop: 'text-weight', type: 'text', placeholder: 'normal | bold | 100-900' },
  { prop: 'caption', type: 'freetext' },
  { prop: 'title', type: 'freetext' },
  { prop: 'credits', type: 'freetext' },
  { prop: 'resource-folder', type: 'freetext' },
];

function styleFieldRow(ctx, def, prefix, getRaw, setRaw) {
  const full = prefix + def.prop;
  const current = getPair(getRaw(), full) ?? '';
  let control;
  let refreshControl = () => {};

  const commit = (value) => {
    const v = value.trim();
    if (v === '') setRaw(removePairs(getRaw(), full));
    else {
      if (def.type === 'freetext') {
        const errCode = styleFreeTextError(v);
        if (errCode) {
          ctx.showError(ctx.t('err-style-' + errCode));
          refreshControl();
          return;
        }
      }
      setRaw(upsertPair(getRaw(), full, v));
    }
    ctx.onBoardChanged();
  };

  if (def.type === 'color') {
    const textInput = el('input', {
      type: 'text', class: 'ssbe-input ssbe-color-text', value: current,
      placeholder: '#RRGGBB', pattern: '#[0-9a-fA-F]{6}',
      onchange: (e) => commit(e.target.value),
    });
    const picker = el('input', {
      type: 'color',
      value: /^#[0-9a-fA-F]{6}$/.test(current) ? current : '#ffffff',
      'aria-label': ctx.t('color-picker-for') + ' ' + full,
      oninput: (e) => { textInput.value = e.target.value; commit(e.target.value); },
    });
    control = el('span', { class: 'ssbe-color-pair' }, [textInput, picker]);
    refreshControl = () => { textInput.value = getPair(getRaw(), full) ?? ''; };
  } else if (def.type === 'select') {
    control = el('select', { class: 'ssbe-input', onchange: (e) => commit(e.target.value) },
      def.options.map((o) => el('option', { value: o, text: o === '' ? ctx.t('style-unset') : o })));
    control.value = def.options.includes(current) ? current : '';
    refreshControl = () => { control.value = getPair(getRaw(), full) ?? ''; };
  } else {
    control = el('input', {
      type: 'text', class: 'ssbe-input', value: current,
      placeholder: def.placeholder ?? (def.type === 'size' ? 'px | NN%' : ''),
      onchange: (e) => commit(e.target.value),
    });
    refreshControl = () => { control.value = getPair(getRaw(), full) ?? ''; };
  }
  return labelled(full, control);
}

function styleGroup(ctx, legendText, defs, prefix, getRaw, setRaw) {
  return el('fieldset', { class: 'ssbe-style-group' }, [
    el('legend', { text: legendText }),
    ...defs.map((d) => styleFieldRow(ctx, d, prefix, getRaw, setRaw)),
  ]);
}

function rawStyleField(ctx, getRaw, setRaw) {
  const ta = el('textarea', {
    class: 'ssbe-input ssbe-raw-style', rows: '3',
    onchange: (e) => {
      if (e.target.value.includes('|')) {
        ctx.showError(ctx.t('err-style-char-pipe'));
        e.target.value = getRaw();
        return;
      }
      setRaw(e.target.value);
      ctx.onBoardChanged();
    },
  });
  ta.value = getRaw();
  return labelled(ctx.t('style-raw-label'), ta);
}

export function buildBoardStylePanel(ctx) {
  const getRaw = () => ctx.state.board.boardStyle;
  const setRaw = (v) => { ctx.state.board.boardStyle = v; };
  return collapsiblePanel(ctx, 'board-style', ctx.t('panel-board-style'), [
    styleGroup(ctx, ctx.t('style-scope-window'), WINDOW_PROPS, '', getRaw, setRaw),
    styleGroup(ctx, ctx.t('style-scope-categories'), GROUP_PROPS, 'category-', getRaw, setRaw),
    styleGroup(ctx, ctx.t('style-scope-phrases'), GROUP_PROPS, 'phrase-', getRaw, setRaw),
    rawStyleField(ctx, getRaw, setRaw),
  ], false);
}

export function buildCategoryStylePanel(ctx) {
  const cat = ctx.selectedCategory();
  if (!cat) return el('div');
  const getRaw = () => cat.styleRaw;
  const setRaw = (v) => { cat.styleRaw = v; };
  const ownProps = [
    { prop: 'background', type: 'color' },
    { prop: 'text-color', type: 'color' },
  ];
  return collapsiblePanel(ctx, 'category-style', ctx.t('panel-category-style'), [
    styleGroup(ctx, ctx.t('style-scope-own'), ownProps, '', getRaw, setRaw),
    styleGroup(ctx, ctx.t('style-scope-phrases'), GROUP_PROPS, 'phrase-', getRaw, setRaw),
    rawStyleField(ctx, getRaw, setRaw),
  ], false);
}

// ---------------------------------------------------------------------------
// Icon fields: text input + emoji popover + ARASAAC assign button
// ---------------------------------------------------------------------------

const EMOJI_SET = [
  '🙂', '😀', '😢', '😡', '😴', '🤒', '😷', '🥰', '🙏', '👍', '👎', '👋',
  '🧍', '👨‍👩‍👧', '👩', '👨', '🍎', '🍞', '🥛', '💧', '☕', '🍽️', '🚻', '🛏️',
  '🚿', '🦷', '👕', '💊', '🩺', '🏠', '🏥', '🚗', '🚌', '📞', '📺', '🎵',
  '📖', '✅', '❌', '❓', '❗', '☀️', '🌧️', '⏰', '❤️', '🕒',
];

function validatedTextInput(ctx, value, validator, apply, extraAttrs = {}) {
  return el('input', {
    type: 'text', class: 'ssbe-input', value, ...extraAttrs,
    onchange: (e) => {
      const v = e.target.value;
      const errCode = validator(v);
      if (errCode) {
        ctx.showError(ctx.t('err-' + errCode) || ctx.t('err-invalid'));
        e.target.value = value; // revert
        return;
      }
      apply(v);
      ctx.onBoardChanged();
    },
  });
}

/**
 * Icon field cluster: validated input + emoji popover + pictogram assign
 * button. `target` identifies the field for ARASAAC assign mode:
 * { kind: 'category'|'phrase', catIndex, phraseIndex? }.
 */
function iconField(ctx, current, apply, target) {
  const input = validatedTextInput(ctx, current, (v) => {
    const e = iconTokenError(v);
    if (e) return 'icon-' + e;
    if (v && v.includes('.') && !isAllowedResourceName(v)) return 'icon-bad-extension';
    return null;
  }, apply, { list: 'ssbe-resource-names', placeholder: ctx.t('icon-placeholder'), class: 'ssbe-input ssbe-icon-input' });

  const emojiGrid = el('div', { class: 'ssbe-emoji-grid', role: 'listbox' },
    EMOJI_SET.map((emo) => el('button', {
      type: 'button', class: 'ssbe-emoji-item', text: emo, 'aria-label': emo,
      onclick: () => {
        apply(emo);
        ctx.onBoardChanged();
      },
    })));
  const emojiPop = el('details', { class: 'ssbe-emoji-pop' }, [
    el('summary', { class: 'ssbe-mini', title: ctx.t('emoji-picker-open'), 'aria-label': ctx.t('emoji-picker-open'), text: '😊' }),
    emojiGrid,
  ]);

  const pictoBtn = el('button', {
    type: 'button', class: 'ssbe-mini', text: '🖼', title: ctx.t('pictogram-picker-open'), 'aria-label': ctx.t('pictogram-picker-open'),
    onclick: () => ctx.armArasaacTarget(target),
  });

  return el('span', { class: 'ssbe-icon-cluster' }, [input, emojiPop, pictoBtn]);
}

function moveButtons(ctx, arr, i, onMoved) {
  const move = (delta) => {
    const j = i + delta;
    if (j < 0 || j >= arr.length) return;
    [arr[i], arr[j]] = [arr[j], arr[i]];
    onMoved(j);
    ctx.onBoardChanged();
  };
  return [
    el('button', { type: 'button', class: 'ssbe-mini', 'aria-label': ctx.t('move-up'), text: '▲', onclick: () => move(-1) }),
    el('button', { type: 'button', class: 'ssbe-mini', 'aria-label': ctx.t('move-down'), text: '▼', onclick: () => move(1) }),
  ];
}

// ---------------------------------------------------------------------------
// Category / phrase panels
// ---------------------------------------------------------------------------

export function buildCategoriesPanel(ctx) {
  const board = ctx.state.board;
  const list = el('div', { class: 'ssbe-list', role: 'list' });

  board.categories.forEach((cat, i) => {
    const row = el('div', { class: 'ssbe-row' + (i === ctx.state.selectedCategory ? ' ssbe-row-selected' : ''), role: 'listitem' });
    row.appendChild(el('button', {
      type: 'button', class: 'ssbe-select-btn', text: (cat.icon && !cat.icon.includes('.') ? cat.icon + ' ' : '') + cat.name,
      'aria-pressed': String(i === ctx.state.selectedCategory),
      onclick: () => { ctx.state.selectedCategory = i; ctx.refresh(); },
    }));
    row.appendChild(iconField(ctx, cat.icon, (v) => { cat.icon = v; }, { kind: 'category', catIndex: i }));
    row.appendChild(validatedTextInput(ctx, cat.name, categoryNameError, (v) => { cat.name = v; },
      { 'aria-label': ctx.t('category-name') }));
    moveButtons(ctx, board.categories, i, (j) => { ctx.state.selectedCategory = j; }).forEach((b) => row.appendChild(b));
    row.appendChild(el('button', {
      type: 'button', class: 'ssbe-mini ssbe-danger', text: '✕', 'aria-label': ctx.t('delete-category'),
      onclick: () => {
        if (!window.confirm(ctx.t('confirm-delete-category'))) return;
        board.categories.splice(i, 1);
        ctx.state.selectedCategory = Math.max(0, Math.min(ctx.state.selectedCategory, board.categories.length - 1));
        ctx.onBoardChanged();
      },
    }));
    list.appendChild(row);
  });

  const addBtn = el('button', {
    type: 'button', class: 'ssbe-btn-secondary', text: ctx.t('add-category'),
    onclick: () => {
      board.categories.push({ icon: '', name: ctx.t('new-category-name'), styleRaw: '', phrases: [] });
      ctx.state.selectedCategory = board.categories.length - 1;
      ctx.state.openPanels['categories'] = true;
      ctx.onBoardChanged();
    },
  });

  return collapsiblePanel(ctx, 'categories', ctx.t('panel-categories'), [list, addBtn], false);
}

export function buildPhrasesPanel(ctx) {
  const cat = ctx.selectedCategory();
  const children = [];
  if (cat) {
    const list = el('div', { class: 'ssbe-list', role: 'list' });
    cat.phrases.forEach((p, i) => {
      const highlighted = ctx.state.highlightPhrase === i;
      const row = el('div', {
        class: 'ssbe-row' + (highlighted ? ' ssbe-row-highlight' : ''),
        role: 'listitem', dataset: { phraseIndex: String(i) },
      });
      row.appendChild(iconField(ctx, p.icon, (v) => { p.icon = v; },
        { kind: 'phrase', catIndex: ctx.state.selectedCategory, phraseIndex: i }));
      row.appendChild(validatedTextInput(ctx, p.text, phraseTextError, (v) => { p.text = v; },
        { 'aria-label': ctx.t('phrase-text'), class: 'ssbe-input ssbe-grow' }));
      row.appendChild(validatedTextInput(ctx, p.audioFile, (v) => {
        if (!v) return null;
        const ext = extOf(v);
        return (ext === 'wav' || ext === 'mp3') ? null : 'audio-bad-extension';
      }, (v) => { p.audioFile = v; }, { list: 'ssbe-audio-names', placeholder: ctx.t('audio-placeholder') }));
      moveButtons(ctx, cat.phrases, i, () => {}).forEach((b) => row.appendChild(b));
      row.appendChild(el('button', {
        type: 'button', class: 'ssbe-mini ssbe-danger', text: '✕', 'aria-label': ctx.t('delete-phrase'),
        onclick: () => { cat.phrases.splice(i, 1); ctx.onBoardChanged(); },
      }));
      list.appendChild(row);
    });
    children.push(list);
    children.push(el('button', {
      type: 'button', class: 'ssbe-btn-secondary', text: ctx.t('add-phrase'),
      onclick: () => { cat.phrases.push({ icon: '', text: ctx.t('new-phrase-text'), audioFile: '' }); ctx.onBoardChanged(); },
    }));
  }
  const title = ctx.t('panel-phrases') + (cat ? ' — ' + cat.name : '');
  return collapsiblePanel(ctx, 'phrases', title, children, true);
}

// ---------------------------------------------------------------------------
// Resources & ARASAAC
// ---------------------------------------------------------------------------

function renameResource(ctx, name) {
  const dot = name.lastIndexOf('.');
  const stem = name.slice(0, dot);
  const ext = name.slice(dot); // extension is locked — type guarantees stay intact
  const input = window.prompt(ctx.t('prompt-new-name'), stem);
  if (input === null) return;
  const newStem = input.trim();
  if (!newStem || /[\\/:*?"<>|=;♫#.]/.test(newStem)) {
    ctx.showError(ctx.t('err-invalid'));
    return;
  }
  const newName = newStem + ext;
  if (newName === name) return;
  if (ctx.state.resources.has(newName)) {
    ctx.showError(ctx.t('err-name-exists'));
    return;
  }
  const bytes = ctx.state.resources.get(name);
  ctx.state.resources.delete(name);
  ctx.state.resources.set(newName, bytes);
  ctx.urls.revoke(name);
  renameResourceReferences(ctx.state.board, name, newName);
  ctx.onBoardChanged();
}

export function buildResourcesPanel(ctx) {
  const { resources } = ctx.state;
  const referenced = ctx.referencedNames();
  const section = el('section', { class: 'ssbe-panel', 'aria-label': ctx.t('panel-resources') });
  section.appendChild(el('h3', { text: ctx.t('panel-resources') }));

  const list = el('ul', { class: 'ssbe-resource-list' });
  for (const [name, bytes] of resources) {
    const used = referenced.has(name);
    list.appendChild(el('li', {}, [
      el('span', { class: 'ssbe-res-name', text: name }),
      el('span', { class: 'ssbe-res-size', text: ctx.formatKb(bytes.length) }),
      el('span', { class: used ? 'ssbe-badge-used' : 'ssbe-badge-unused', text: ctx.t(used ? 'res-used' : 'res-unused') }),
      el('button', {
        type: 'button', class: 'ssbe-mini', text: '✎', 'aria-label': ctx.t('rename-resource') + ' ' + name,
        title: ctx.t('rename-resource'),
        onclick: () => renameResource(ctx, name),
      }),
      el('button', {
        type: 'button', class: 'ssbe-mini ssbe-danger', text: '✕', 'aria-label': ctx.t('delete-resource') + ' ' + name,
        onclick: () => { resources.delete(name); ctx.urls.revoke(name); ctx.onBoardChanged(); },
      }),
    ]));
  }
  section.appendChild(list);

  const fileInput = el('input', {
    type: 'file', accept: '.ico,.png,.jpg,.jpeg,.wav,.mp3', multiple: 'multiple', class: 'ssbe-file',
    onchange: async (e) => {
      for (const f of e.target.files) await ctx.addResourceFile(f);
      e.target.value = '';
    },
  });
  section.appendChild(labelled(ctx.t('add-resources'), fileInput));

  const iconList = el('datalist', { id: 'ssbe-resource-names' });
  const audioList = el('datalist', { id: 'ssbe-audio-names' });
  for (const name of resources.keys()) {
    const ext = extOf(name);
    if (ext === 'wav' || ext === 'mp3') audioList.appendChild(el('option', { value: name }));
    else iconList.appendChild(el('option', { value: name }));
  }
  section.append(iconList, audioList);
  return section;
}

export function buildArasaacPanel(ctx) {
  const cfg = ctx.data.settings.arasaac;

  if (!cfg.enabled) {
    return collapsiblePanel(ctx, 'arasaac', ctx.t('panel-arasaac'), [
      el('p', { class: 'ssbe-note', text: ctx.t('arasaac-disabled') }),
    ], false);
  }

  const ara = ctx.state.ara; // { term, locale, results }
  const results = el('div', { class: 'ssbe-ara-grid', role: 'list' });
  const status = el('p', { class: 'ssbe-note', role: 'status' });

  const renderResults = (items) => {
    results.replaceChildren();
    for (const item of items ?? []) {
      const img = el('img', { src: ctx.arasaacThumb(item.id), alt: item.keyword, loading: 'lazy' });
      results.appendChild(el('button', {
        type: 'button', class: 'ssbe-ara-item', title: item.keyword,
        onclick: () => ctx.pickArasaac(item),
      }, [img]));
    }
  };
  if (ara.results) renderResults(ara.results);

  const localeSel = el('select', { class: 'ssbe-input', onchange: (e) => { ara.locale = e.target.value; } },
    ctx.arasaacLocales().map((l) => el('option', { value: l, text: l })));
  localeSel.value = ara.locale || cfg.locale || 'en';

  const searchInput = el('input', {
    type: 'search', class: 'ssbe-input ssbe-grow', placeholder: ctx.t('arasaac-search-placeholder'),
    value: ara.term, oninput: (e) => { ara.term = e.target.value; },
  });
  const doSearch = async () => {
    status.textContent = ctx.t('arasaac-searching');
    results.replaceChildren();
    try {
      const items = await ctx.searchArasaac(localeSel.value, searchInput.value);
      ara.results = items;
      status.textContent = items.length ? '' : ctx.t('arasaac-no-results');
      renderResults(items);
    } catch {
      status.textContent = ctx.t('arasaac-error');
    }
  };
  searchInput.addEventListener('keydown', (e) => { if (e.key === 'Enter') { e.preventDefault(); doSearch(); } });

  const children = [];
  const target = ctx.state.arasaacTarget;
  if (target) {
    const what = target.kind === 'category' ? ctx.t('arasaac-target-category') : ctx.t('arasaac-target-phrase');
    children.push(el('p', { class: 'ssbe-note ssbe-ara-target', text: ctx.t('arasaac-target') + ' ' + what }));
  }
  children.push(el('div', { class: 'ssbe-row' }, [
    searchInput, localeSel,
    el('button', { type: 'button', class: 'ssbe-btn-secondary', text: ctx.t('arasaac-search'), onclick: doSearch }),
  ]));
  children.push(status, results);
  children.push(el('p', { class: 'ssbe-attribution', text: cfg.attribution }));

  return collapsiblePanel(ctx, 'arasaac', ctx.t('panel-arasaac'), children, false);
}

// ---------------------------------------------------------------------------
// Validation findings
// ---------------------------------------------------------------------------

export function buildFindingsPanel(ctx, findings) {
  const section = el('section', { class: 'ssbe-panel', 'aria-label': ctx.t('panel-findings') });
  section.appendChild(el('h3', { text: ctx.t('panel-findings') }));
  if (!findings.length) {
    section.appendChild(el('p', { class: 'ssbe-note', text: ctx.t('findings-none') }));
    return section;
  }
  const ul = el('ul', { class: 'ssbe-findings' });
  for (const f of findings) {
    const msg = (ctx.t('finding-' + f.code) || f.code) + (f.detail ? ` — ${f.detail}` : '');
    ul.appendChild(el('li', { class: f.level === 'error' ? 'ssbe-finding-error' : 'ssbe-finding-warning', text: msg }));
  }
  section.appendChild(ul);
  return section;
}
