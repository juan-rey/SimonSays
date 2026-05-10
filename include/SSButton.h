/*
   SSButton.h: SSButton custom owner-drawn button class header file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSButton_h
#define SSButton_h

#include "stdafx.h"

#define SSBUTTON_CLASS L"SSButton"
#define SSBUTTON_ICON_DEFAULT_SIZE 64

// Background source for the button surface
enum class SSButtonBackground
{
  Default,       // System COLOR_BTNFACE (GetSysColor)
  Color,         // Custom COLORREF supplied by caller
};

// Text color source
enum class SSButtonTextColor
{
  Default, // System COLOR_BTNTEXT
  Custom   // Custom COLORREF supplied by caller
};

// Border/frame style
enum class SSButtonBorderStyle
{
  Square,        // Raised/sunken rectangle via DrawEdge; flat 1-px rect when BS_FLAT is set
  Rounded,       // Rounded rectangle via RoundRect
};

// Optional icon rendered alongside the label
enum class SSButtonIconType
{
  None,         // Text only
  Emoji,        // Unicode emoji drawn with the Segoe UI Emoji font
  StandardIcon  // Win32 HICON drawn with DrawIconEx
};

// Where the icon is placed relative to the label.
// Left/Right are interpreted in client coordinates; with WS_EX_LAYOUTRTL on
// the window GDI mirrors the DC, so logical Left renders visually on the right.
enum class SSButtonIconPosition
{
  Left,    // Icon to the left of text (default)
  Right,   // Icon to the right of text
  Top,     // Icon above text
  Bottom   // Icon below text
};

// Full configuration bundle; all fields have safe defaults
struct SSButtonConfig
{
  // Background
  SSButtonBackground bgType = SSButtonBackground::Default;
  COLORREF           bgColor = CLR_NONE; // used when bgType == Color

  // Text color
  SSButtonTextColor textColorType = SSButtonTextColor::Default;
  COLORREF          textColor = RGB( 0, 0, 0 );

  // Border
  SSButtonBorderStyle borderStyle = SSButtonBorderStyle::Square;
  int                 cornerRadius = 6; // pixels, used when borderStyle == Rounded
  int                 borderWidth = 2; // thickness of the border 0, 1 or 2 looks good depending on the button size

  // Icon
  SSButtonIconType     iconType     = SSButtonIconType::None;
  SSButtonIconPosition iconPosition = SSButtonIconPosition::Left;
  std::wstring         emoji;               // used when iconType == Emoji
  std::wstring         iconFileFullPath;    // used when iconType == StandardIcon
  int                  iconSize     = 0;    // square pixel size; 0 == auto-fit content height
  int                  iconPadding  = 2;    // space between the icon and border/text in pixels
};

// Self-contained custom-drawn button.
// Wraps a Win32 HWND of the registered "SSButton" window class.
//
// Style flags honored on creation (read from the `style`/`exStyle` arguments
// to Create() and from later WM_STYLECHANGED notifications):
//   WS_TABSTOP, WS_VISIBLE, WS_CHILD, BS_PUSHBUTTON, BS_MULTILINE, BS_FLAT,
//   BS_LEFT, BS_RIGHT, BS_CENTER       — horizontal text alignment
//   BS_TOP,  BS_BOTTOM, BS_VCENTER     — vertical text alignment (single AND multi-line)
//   WS_EX_LAYOUTRTL, WS_EX_RTLREADING  — right-to-left layout (GDI auto-mirrors)
// Defaults when no horizontal/vertical alignment flag is set: centered both axes.
class SSButton
{
public:
  SSButton();
  ~SSButton();

  // Moveable but not copyable: the HWND is unique and GWLP_USERDATA must track the live address.
  SSButton( SSButton && other ) noexcept;
  SSButton & operator=( SSButton && ) = delete;
  SSButton( const SSButton & ) = delete;
  SSButton & operator=( const SSButton & ) = delete;

  // Creates the underlying HWND and returns it (nullptr on failure).
  // The SSButton instance must outlive the parent window.
  HWND Create( HWND hwndParent, HINSTANCE hInstance, int id,
    const std::wstring & text,
    int x, int y, int w, int h,
    DWORD style, DWORD exStyle = 0,
    const SSButtonConfig & config = {} );

  const SSButtonConfig & GetConfig() const { return m_config; }
  // Updates the visual configuration and repaints.
  void SetConfig( const SSButtonConfig & config );

  void SetColors( COLORREF backgroundColor, COLORREF textColor = CLR_NONE, COLORREF hoverColor = CLR_NONE, COLORREF pressedColor = CLR_NONE, COLORREF disabledColor = CLR_NONE ); // textColor is only used when textColorType == Custom; pass CLR_NONE to keep existing
  void SetSystemColors( bool includeTextColor = false ); // resets to system colors; includeTextColor also resets text color to COLOR_BTNTEXT

  // Icon setters for common cases; each updates the config and repaints.
  void SetIcon( const std::wstring & iconFileFullPath, int iconSize = 0, bool updateIconPosition = false, SSButtonIconPosition iconPosition = SSButtonIconPosition::Left ); // updates config.iconType to StandardIcon
  void SetEmoji( const std::wstring & emoji, int iconSize = 0, bool updateIconPosition = false, SSButtonIconPosition iconPosition = SSButtonIconPosition::Left );           // updates config.iconType to Emoji
  void NoIcon();

  // Stores the font used to render the label (does NOT take ownership) and
  // notifies the HWND via WM_SETFONT so any subsequent WM_GETFONT also matches.
  void SetFont( HFONT hFont, bool redraw = true );

  // Wrappers around common Win32 calls so callers don't need a raw HWND.
  // Each is a no-op when the underlying HWND is null.
  void SetText( const std::wstring & text );
  const std::wstring & GetText() const { return m_text; }
  const wchar_t * GetTextCStr() const { return m_text.c_str(); }
  void SetStyle( DWORD style, bool reframe = true );  // overwrites GWL_STYLE; reframe issues SWP_FRAMECHANGED
  void SetPos( int x, int y, int w, int h, UINT swpFlags = SWP_NOZORDER | SWP_NOACTIVATE );
  void Invalidate( bool eraseBackground = true );
  void SetFocus();
  void SetEnabled( bool enabled );
  bool IsEnabled() const;
  void Show( bool show = true );
  void Hide() { Show( false ); }
  void Destroy();

  // Registers the "SSButton" WNDCLASSEX once per process.
  // Called automatically by Create(); safe to call multiple times.
  static bool RegisterWindowClass( HINSTANCE hInstance );

  // Releases process-wide D2D/DWrite/GDI resources cached by the class.
  // Call once at app shutdown (after the last SSButton is destroyed).
  static void ReleaseSharedResources();

private:
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

  void Paint( HWND hwnd );
  void FireClick( HWND hwnd );
  COLORREF ResolvedTextColor( bool isEnabled ) const;
  COLORREF ResolvedBgColor( bool isEnabled ) const;
  void ReleaseIcon();

  HWND           m_hwnd = nullptr;
  HFONT          m_hExternalFont = nullptr; // not owned, set via WM_SETFONT or SetFont(); may be null
  HICON          m_hIcon = nullptr;
  SSButtonConfig m_config;
  DWORD          m_style = 0;
  DWORD          m_exStyle = 0;

  std::wstring m_text; // cached text for quick access in Paint; kept in sync with the HWND text via WM_SETTEXT

  bool m_pressed = false;
  bool m_hovered = false;
  bool m_focused = false;
  bool m_trackMouse = false;
  bool m_keyDown = false; // space/enter held

  COLORREF m_backgroundColor = RGB( 240, 240, 240 ); // Default COLOR_BTNFACE or custom when bgType == Color
  COLORREF m_hoverColor = CLR_NONE;    // derived from m_backgroundColor or custom; used when bgType == Color and hovered
  COLORREF m_pressedColor = CLR_NONE;  // derived from m_backgroundColor or custom; used when bgType == Color and pressed
  COLORREF m_disabledColor = CLR_NONE; // derived from m_backgroundColor or custom; used when bgType == Color and !isEnabled

  COLORREF m_textColor = CLR_NONE;     // used to optimize text color calculation when textColorType == Custom

  COLORREF m_highlightBorderColor = RGB( 255, 255, 255 );  // Default COLOR_BTNHIGHLIGHT or background with brightness factor 1.40 to 1.50
  COLORREF m_lightBorderColor = RGB( 227, 227, 227 );      // Default COLOR_3DLIGHT or background with brightness factor 1.15 to 1.20
  COLORREF m_shadowBorderColor = RGB( 160, 160, 160 );     // Default COLOR_BTNSHADOW or background with brightness factor 0.70 to 0.80
  COLORREF m_darkShadowBorderColor = RGB( 105, 105, 105 ); // Default COLOR_3DDKSHADOW or background with brightness factor 0.50 to 0.60
};

#endif // SSButton_h
