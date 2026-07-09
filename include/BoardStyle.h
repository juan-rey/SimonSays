/*
   BoardStyle.h: board & category style parsing and cascade resolution.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef BoardStyle_h
#define BoardStyle_h

#include "stdafx.h"

// Style model for the category window (see docs/specs/board-style.spec.md).
// Parses "property:value;" lists into typed properties with per-property
// "set" flags, and resolves the cascade built-in default -> board -> category.
// Deliberately UI-free: no SSButton/window types here; hosts map the resolved
// properties onto SSButtonConfig / fonts / brushes themselves.

// A size value: absolute pixels, or a percentage of the built-in default
// (the only relative form, per STY-F02).
struct StyleSize
{
  int value = 0;
  bool isPercent = false;
  bool set = false;
};

enum class StyleIconPos { NotSet, Left, Right, Top, Bottom };
enum class StyleHAlign { NotSet, Left, Center, Right };
enum class StyleVAlign { NotSet, Top, Middle, Bottom };

// One selector's worth of properties. Window scope uses background /
// separatorColor / textColor / fontName / fontSize; button groups use
// everything except separatorColor.
struct StyleProps
{
  COLORREF background = 0;      bool hasBackground = false;
  COLORREF textColor = 0;       bool hasTextColor = false;
  COLORREF separatorColor = 0;  bool hasSeparatorColor = false;

  StyleSize width;
  StyleSize height;
  StyleSize margin;
  StyleSize cornerRadius;
  StyleSize borderWidth;
  StyleSize iconSize;
  StyleSize fontSize;

  std::wstring fontName;        // empty = not set

  StyleIconPos iconPosition = StyleIconPos::NotSet;
  StyleHAlign textHAlign = StyleHAlign::NotSet;
  StyleVAlign textVAlign = StyleVAlign::NotSet;
};

// Board (global) layer: window + all-category-buttons + all-phrase-buttons.
struct BoardStyle
{
  StyleProps window;
  StyleProps categoryButtons;
  StyleProps phraseButtons;
};

// Per-category layer: the category's own button colors + its phrase buttons.
// Own-button colors also serve as the base for the phrase buttons (STY-F12);
// that flow-through is applied by the resolver caller, not stored here.
struct CategoryStyle
{
  StyleProps ownButton;
  StyleProps phraseButtons;
};

// Parses a board style list ("background:#FBFAFD;phrase-height:120%;...").
// Unprefixed names are window properties; "category-"/"phrase-" prefixes route
// to the button groups. Unknown/malformed pairs are skipped (STY-F03); a later
// duplicate overrides an earlier one (STY-F04). Never throws.
void ParseBoardStyleList( const std::wstring & list, BoardStyle & out );

// Parses a category style list ("background:#FFD966;phrase-corner-radius:8;").
// Unprefixed "background"/"text-color" go to the own button; "phrase-" prefixed
// properties go to the phrase buttons; anything else is skipped (STY-F30).
void ParseCategoryStyleList( const std::wstring & list, CategoryStyle & out );

// Returns base with every property that is set in `over` overridden (STY-F10).
StyleProps ResolveEffectiveStyle( const StyleProps & base, const StyleProps & over );

// Resolves a size against its built-in default: unset -> default; percent ->
// percentage of the default; absolute -> the value (pixels). Zero is honored
// only where meaningful (allowZero: margins, border width, corner radius,
// icon size); layout dimensions fall back to the default instead (STY-F45).
int ResolveStyleSize( const StyleSize & size, int defaultValue, bool allowZero = false );

// Black or white, whichever contrasts with the given background — used when a
// background is styled but no text color resolves anywhere (STY-F41).
COLORREF ContrastTextColorFor( COLORREF background );

#endif // BoardStyle_h
