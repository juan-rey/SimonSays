/*
   SettingsDialog.cpp: modal Settings dialog (F2).

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SettingsDialog.h"
#include "RegistryManager.h"
#include "PlaybackEngine.h"
#include "resource.h"
#include "utils.h"
#include "localized_strings.h"
#include <windows.h>
#include <commctrl.h>
#include <sapi.h>
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(default: 4996)

struct SettingsDialogContext
{
  Settings tempSettings;
  std::vector<VoiceInfo> voices;
  std::vector<LanguageInfo> languages;
  std::vector<AudioOutputDevice> outputDevices; // "Sound output" list (SET-F15)
  HWND hListenTooltip = NULL;
  bool noVoiceForLanguage = false;
  bool accepted = false;
};

// Item data of the "Sound output" combo: an index into outputDevices, or one of these.
#define OUTPUT_DEVICE_ITEM_DEFAULT ( (LPARAM) -1 )      // Windows default
#define OUTPUT_DEVICE_ITEM_DISCONNECTED ( (LPARAM) -2 ) // saved device, not connected now

static void PopulateLanguageCombo( HWND hDlg, SettingsDialogContext * ctx )
{
  if( ctx->languages.empty() )
  {
    ctx->languages.assign( SUPPORTED_LANGUAGES.begin(), SUPPORTED_LANGUAGES.end() );
  }

  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
  if( !hCombo ) return;

  if( ctx->tempSettings.language.empty() )
  {
    ctx->tempSettings.language = GetSystemLanguage();
  }

  int selectedIndex = -1;
  for( size_t i = 0; i < ctx->languages.size(); ++i )
  {
    const auto & info = ctx->languages[i];
    int idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) info.NativeName.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, (LPARAM) i );
    if( selectedIndex == -1 && _wcsicmp( info.EnglishName.c_str(), ctx->tempSettings.language.c_str() ) == 0 )
    {
      selectedIndex = idx;
    }
  }

  if( selectedIndex == -1 && SendMessage( hCombo, CB_GETCOUNT, 0, 0 ) > 0 )
  {
    selectedIndex = 0;
  }

  if( selectedIndex != -1 )
  {
    SendMessage( hCombo, CB_SETCURSEL, selectedIndex, 0 );
  }
}

static std::wstring GetSelectedLanguageForLocalization( HWND hDlg, SettingsDialogContext * ctx )
{
  if( !ctx ) return L"";
  HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
  if( hLanguageCombo )
  {
    int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
    if( sel != CB_ERR )
    {
      size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
      if( langIndex < ctx->languages.size() )
      {
        return ctx->languages[langIndex].EnglishName;
      }
    }
  }
  return ctx->tempSettings.language;
}

// Fills the "Sound output" combo (settings.spec.md SET-F15): Windows default,
// then the devices from EnumerateOutputDevices (virtual cables first), and the
// selected device as "<name> (not connected)" when it is not present, so
// opening Settings and pressing OK never silently drops the choice.
static void PopulateOutputDeviceCombo( HWND hDlg, SettingsDialogContext * ctx, const std::wstring & language,
  const std::wstring & selectedId, const std::wstring & selectedName )
{
  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO );
  if( !hCombo ) return;
  SendMessage( hCombo, CB_RESETCONTENT, 0, 0 );

  int idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) GetLocalizedString( SETTINGS_OUTPUT_DEVICE_DEFAULT_ID, language ) );
  SendMessage( hCombo, CB_SETITEMDATA, idx, OUTPUT_DEVICE_ITEM_DEFAULT );
  int selectedIndex = idx;
  bool selectedFound = selectedId.empty();

  for( size_t i = 0; i < ctx->outputDevices.size(); ++i )
  {
    const AudioOutputDevice & device = ctx->outputDevices[i];
    bool isSelected = !selectedId.empty() && _wcsicmp( device.id.c_str(), selectedId.c_str() ) == 0;
#ifdef SETTINGS_OUTPUT_DEVICE_SHORT_LIST
    if( !device.isVirtualCable && !isSelected ) continue;
#endif
    idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) device.name.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, (LPARAM) i );
    if( isSelected )
    {
      selectedIndex = idx;
      selectedFound = true;
    }
  }

  if( !selectedFound )
  {
    std::wstring label = ( selectedName.empty() ? selectedId : selectedName ) + L" " + GetLocalizedString( SETTINGS_OUTPUT_DEVICE_NOT_CONNECTED_ID, language );
    idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) label.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, OUTPUT_DEVICE_ITEM_DISCONNECTED );
    selectedIndex = idx;
  }

  SendMessage( hCombo, CB_SETCURSEL, selectedIndex, 0 );
}

// Reads the "Sound output" combo; empty id = Windows default.
static void GetSelectedOutputDevice( HWND hDlg, SettingsDialogContext * ctx, std::wstring & id, std::wstring & name )
{
  id = ctx->tempSettings.outputDeviceId;
  name = ctx->tempSettings.outputDeviceName;
  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO );
  int sel = hCombo ? (int) SendMessage( hCombo, CB_GETCURSEL, 0, 0 ) : CB_ERR;
  if( sel == CB_ERR ) return;

  LPARAM data = SendMessage( hCombo, CB_GETITEMDATA, sel, 0 );
  if( data == OUTPUT_DEVICE_ITEM_DISCONNECTED )
    return; // keep the saved, currently disconnected device
  if( data >= 0 && (size_t) data < ctx->outputDevices.size() )
  {
    id = ctx->outputDevices[(size_t) data].id;
    name = ctx->outputDevices[(size_t) data].name;
  }
  else
  {
    id.clear();
    name.clear();
  }
}

static void UpdateSettingsDialogLocalization( HWND hDlg, const std::wstring & language )
{
  if( !hDlg ) return;
  bool isRtl = IsLanguageRTL( language );
  updateRtlExStyle( hDlg, isRtl );
  EnumChildWindows( hDlg, ApplyRtlStylesCallback, isRtl );

  SetWindowText( hDlg, GetLocalizedString( SETTINGS_DIALOG_TITLE_TEXT_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_DEFAULT_TEXT, GetLocalizedString( SETTINGS_DEFAULT_TEXT_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, GetLocalizedString( SETTINGS_USE_DEFAULT_TEXT_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, GetLocalizedString( SETTINGS_SHOW_QUICK_BUTTONS_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_LANGUAGE, GetLocalizedString( SETTINGS_LANGUAGE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOICE, GetLocalizedString( SETTINGS_VOICE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_TEST_VOICE, GetLocalizedString( SETTINGS_TEST_VOICE_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOLUME, GetLocalizedString( SETTINGS_VOLUME_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_RATE, GetLocalizedString( SETTINGS_RATE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, GetLocalizedString( SETTINGS_SPEAK_ON_CLICK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_AUTORESIZE_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, GetLocalizedString( SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, GetLocalizedString( SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, GetLocalizedString( SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, GetLocalizedString( SETTINGS_SHOW_TOUCH_KEYBOARD_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_OUTPUT_DEVICE, GetLocalizedString( SETTINGS_OUTPUT_DEVICE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LISTEN_SETTINGS, GetLocalizedString( SETTINGS_LISTEN_SETTINGS_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDOK, GetLocalizedString( SETTINGS_OK_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_HELP, GetLocalizedString( SETTINGS_HELP_BUTTON_ID, language ) );

  SettingsDialogContext * ctx = reinterpret_cast<SettingsDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
  if( ctx )
  {
    // Re-fill the output list so "Windows default" / "(not connected)" follow the language, keeping the selection.
    std::wstring id = ctx->tempSettings.outputDeviceId;
    std::wstring name = ctx->tempSettings.outputDeviceName;
    if( SendDlgItemMessage( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO, CB_GETCOUNT, 0, 0 ) > 0 )
      GetSelectedOutputDevice( hDlg, ctx, id, name );
    PopulateOutputDeviceCombo( hDlg, ctx, language, id, name );

    if( ctx->hListenTooltip )
    {
      TOOLINFO ti = {};
      ti.cbSize = TTTOOLINFOW_V2_SIZE;
      ti.hwnd = hDlg;
      ti.uId = (UINT_PTR) GetDlgItem( hDlg, IDC_SETTINGS_LISTEN_SETTINGS );
      ti.lpszText = (LPWSTR) GetLocalizedString( SETTINGS_LISTEN_SETTINGS_TOOLTIP_ID, language );
      SendMessage( ctx->hListenTooltip, TTM_UPDATETIPTEXT, 0, (LPARAM) &ti );
    }
  }
}

static INT_PTR CALLBACK SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  SettingsDialogContext * ctx = nullptr;
  if( message == WM_INITDIALOG )
  {
    ctx = reinterpret_cast<SettingsDialogContext *>( lParam );
    if( !ctx ) return FALSE;
    SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );
  }
  else
  {
    ctx = reinterpret_cast<SettingsDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
  }

  if( ctx )
  {
    switch( message )
    {
      case WM_INITDIALOG:
      {
        // Lift the control's 30,000-character default (CAT-N06).
        SendDlgItemMessage( hDlg, IDC_SETTINGS_DEFAULT_TEXT, EM_SETLIMITTEXT, 0, 0 );
        SetDlgItemText( hDlg, IDC_SETTINGS_DEFAULT_TEXT, ctx->tempSettings.defaultText.c_str() );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, BM_SETCHECK,
          ctx->tempSettings.useDefaultText ? BST_CHECKED : BST_UNCHECKED, 0 );

        if( ctx->tempSettings.voice.empty() )
        {
          LPWSTR pszTokenId = NULL;
          if( SUCCEEDED( SpGetDefaultTokenIdFromCategoryId( SPCAT_VOICES, &pszTokenId ) ) )
          {
            ctx->tempSettings.voice = pszTokenId;
            CoTaskMemFree( pszTokenId );
          }
        }

        HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
        int selectedIndex = -1;
        for( size_t i = 0; i < ctx->voices.size(); ++i )
        {
          int idx = (int) SendMessage( hVoiceCombo, CB_ADDSTRING, 0, (LPARAM) ctx->voices[i].name.c_str() );
          SendMessage( hVoiceCombo, CB_SETITEMDATA, idx, (LPARAM) i );
          if( selectedIndex == -1 && ctx->tempSettings.voice == ctx->voices[i].key )
          {
            selectedIndex = idx;
          }
        }

        if( SendMessage( hVoiceCombo, CB_GETCOUNT, 0, 0 ) > 0 )
        {
          SendMessage( hVoiceCombo, CB_SETCURSEL, selectedIndex == -1 ? 0 : selectedIndex, 0 );
        }

        PopulateLanguageCombo( hDlg, ctx );

        int clampedVolume = CLAMPED_VOICE_VOLUME( ctx->tempSettings.volume );
        ConfigureSlider( hDlg, IDC_SETTINGS_VOLUME_SLIDER, SIMONSAYS_SETTINGS_MIN_VOICE_VOLUME, SIMONSAYS_SETTINGS_MAX_VOICE_VOLUME, clampedVolume );
        SyncSliderToEdit( hDlg, IDC_SETTINGS_VOLUME_SLIDER, IDC_SETTINGS_VOLUME_EDIT, FALSE );

        int clampedRate = CLAMPED_VOICE_RATE( ctx->tempSettings.rate );
        ConfigureSlider( hDlg, IDC_SETTINGS_RATE_SLIDER, SIMONSAYS_SETTINGS_MIN_VOICE_RATE, SIMONSAYS_SETTINGS_MAX_VOICE_RATE, clampedRate );
        SyncSliderToEdit( hDlg, IDC_SETTINGS_RATE_SLIDER, IDC_SETTINGS_RATE_EDIT, TRUE );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, BM_SETCHECK,
          ctx->tempSettings.speakDirectlyWhenClickingPhrase ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, BM_SETCHECK,
          ctx->tempSettings.rememberCategoryWindowSize ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, BM_SETCHECK,
          ctx->tempSettings.autoresizeCategoryWindow ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, BM_SETCHECK,
          ctx->tempSettings.minimizeCategoryWindowAutomatically ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, BM_SETCHECK,
          ctx->tempSettings.increaseVolumeWhenPlaying ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, BM_SETCHECK,
          ctx->tempSettings.reduceOtherAudioWhenPlaying ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, BM_SETCHECK,
          ctx->tempSettings.stopPreviousPlayback ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, BM_SETCHECK,
          ctx->tempSettings.showTouchKeyboard ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, BM_SETCHECK,
          ctx->tempSettings.showQuickButtons ? BST_CHECKED : BST_UNCHECKED, 0 );

        // "Sound output" list (filled by UpdateSettingsDialogLocalization) and the
        // "Listen settings..." tooltip explaining what to do in the Windows dialog.
        ctx->outputDevices = PlaybackEngine::EnumerateOutputDevices();
        ctx->hListenTooltip = CreateWindowEx( 0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hDlg, NULL, GetModuleHandle( NULL ), NULL );
        if( ctx->hListenTooltip )
        {
          TOOLINFO ti = {};
          ti.cbSize = TTTOOLINFOW_V2_SIZE;
          ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
          ti.hwnd = hDlg;
          ti.uId = (UINT_PTR) GetDlgItem( hDlg, IDC_SETTINGS_LISTEN_SETTINGS );
          ti.lpszText = (LPWSTR) L"";
          SendMessage( ctx->hListenTooltip, TTM_ADDTOOL, 0, (LPARAM) &ti );
          SendMessage( ctx->hListenTooltip, TTM_SETMAXTIPWIDTH, 0, 320 );
          SendMessage( ctx->hListenTooltip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 30000 );
        }

        UpdateSettingsDialogLocalization( hDlg, GetSelectedLanguageForLocalization( hDlg, ctx ) );
        return TRUE;
      }

      case WM_COMMAND:
      {
        WORD controlId = LOWORD( wParam );
        WORD notifyCode = HIWORD( wParam );

        if( notifyCode == EN_CHANGE )
        {
          HWND hFocus = GetFocus();
          if( controlId == IDC_SETTINGS_VOLUME_EDIT && hFocus == GetDlgItem( hDlg, IDC_SETTINGS_VOLUME_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_SETTINGS_VOLUME_EDIT, IDC_SETTINGS_VOLUME_SLIDER, FALSE, SIMONSAYS_SETTINGS_MIN_VOICE_VOLUME, SIMONSAYS_SETTINGS_MAX_VOICE_VOLUME );
            return TRUE;
          }
          if( controlId == IDC_SETTINGS_RATE_EDIT && hFocus == GetDlgItem( hDlg, IDC_SETTINGS_RATE_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_SETTINGS_RATE_EDIT, IDC_SETTINGS_RATE_SLIDER, TRUE, SIMONSAYS_SETTINGS_MIN_VOICE_RATE, SIMONSAYS_SETTINGS_MAX_VOICE_RATE );
            return TRUE;
          }
        }
        if( controlId == IDC_SETTINGS_LANGUAGE_COMBO && notifyCode == CBN_SELCHANGE )
        {
          std::wstring previous_voice = ctx->tempSettings.voice;

          HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
          if( hVoiceCombo )
          {
            int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
            if( sel != CB_ERR )
            {
              size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
              if( voiceIndex < ctx->voices.size() )
              {
                if( !ctx->voices[voiceIndex].key.empty() )
                {
                  previous_voice = ctx->voices[voiceIndex].key;
                }
              }
            }
          }

          UpdateSettingsDialogLocalization( hDlg, GetSelectedLanguageForLocalization( hDlg, ctx ) );
          ctx->voices = RegistryManager::PopulateAvaibleVoicesFromRegistry( GetSelectedLanguageForLocalization( hDlg, ctx ) );
          if( ctx->voices.empty() )
          {
            ctx->noVoiceForLanguage = true;
            ctx->voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
          }
          else
          {
            ctx->noVoiceForLanguage = false;
          }

          SendMessage( hVoiceCombo, CB_RESETCONTENT, 0, 0 );

          int selectedIndex = -1;
          for( size_t i = 0; i < ctx->voices.size(); ++i )
          {
            int idx = (int) SendMessage( hVoiceCombo, CB_ADDSTRING, 0, (LPARAM) ctx->voices[i].name.c_str() );
            SendMessage( hVoiceCombo, CB_SETITEMDATA, idx, (LPARAM) i );
            if( previous_voice == ctx->voices[i].key )
            {
              selectedIndex = idx;
            }
            else if( selectedIndex == -1 )
            {
              if( ctx->tempSettings.voice == ctx->voices[i].key )
              {
                selectedIndex = idx;
              }
            }
          }

          if( SendMessage( hVoiceCombo, CB_GETCOUNT, 0, 0 ) > 0 )
          {
            SendMessage( hVoiceCombo, CB_SETCURSEL, selectedIndex == -1 ? 0 : selectedIndex, 0 );
          }

          return TRUE;
        }

        switch( controlId )
        {
          case IDC_SETTINGS_TEST_VOICE:
          {
            std::wstring sampleText;
            HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
            if( hLanguageCombo )
            {
              int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
              if( sel != CB_ERR )
              {
                size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
                if( langIndex < ctx->languages.size() )
                {
                  sampleText = ctx->languages[langIndex].VoiceTestSampleText;
                }
              }
            }

            if( sampleText.empty() )
            {
              sampleText = ctx->tempSettings.defaultText;
            }
            if( sampleText.empty() )
            {
              sampleText = L"This is a sample phrase.";
            }

            std::wstring voiceKey = ctx->tempSettings.voice;
            HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
            if( hVoiceCombo )
            {
              int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
              if( sel != CB_ERR )
              {
                size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
                if( voiceIndex < ctx->voices.size() )
                {
                  voiceKey = ctx->voices[voiceIndex].key;
                }
              }
            }

            // Configure the preview voice on the UI thread, then hand ownership to a
            // detached worker so the synchronous Speak/WaitUntilDone doesn't freeze
            // the settings dialog. The worker is responsible for releasing the voice.
            ISpVoice * previewVoice = nullptr;
            if( SUCCEEDED( CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &previewVoice ) ) && previewVoice )
            {
              if( !voiceKey.empty() )
              {
                ISpObjectToken * token = nullptr;
                if( SUCCEEDED( SpGetTokenFromId( voiceKey.c_str(), &token, FALSE ) ) )
                {
                  previewVoice->SetVoice( token );
                  token->Release();
                }
              }

              int volumeValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_VOLUME_SLIDER, TBM_GETPOS, 0, 0 );
              previewVoice->SetVolume( CLAMPED_VOICE_VOLUME( volumeValue ) );

              int rateValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_RATE_SLIDER, TBM_GETPOS, 0, 0 );
              previewVoice->SetRate( CLAMPED_VOICE_RATE( rateValue ) );

              // Preview on the device chosen in the dialog, so the cable routing can be checked from here.
              std::wstring outputId, outputName;
              GetSelectedOutputDevice( hDlg, ctx, outputId, outputName );
              UINT waveOutId = 0;
              if( PlaybackEngine::FindWaveOutDeviceId( outputId, waveOutId ) )
              {
                ISpMMSysAudio * pAudio = nullptr;
                if( SUCCEEDED( CoCreateInstance( CLSID_SpMMAudioOut, nullptr, CLSCTX_ALL, IID_ISpMMSysAudio, (void **) &pAudio ) ) && pAudio )
                {
                  if( SUCCEEDED( pAudio->SetDeviceId( waveOutId ) ) )
                    previewVoice->SetOutput( pAudio, TRUE );
                  pAudio->Release();
                }
              }

              std::thread( [previewVoice, sample = std::move( sampleText )]()
                {
                  CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
                  previewVoice->Speak( sample.c_str(), SPF_IS_NOT_XML, nullptr );
                  previewVoice->WaitUntilDone( INFINITE );
                  previewVoice->Release();
                  CoUninitialize();
                } ).detach();
            }
            return TRUE;
          }

          case IDOK:
          {
            // Sized from the control, not a fixed buffer: a pasted default
            // text longer than the old 1024 wchar was silently truncated.
            ctx->tempSettings.defaultText = ReadEditControlText( GetDlgItem( hDlg, IDC_SETTINGS_DEFAULT_TEXT ) );
            ctx->tempSettings.useDefaultText = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, BM_GETCHECK, 0, 0 ) == BST_CHECKED );

            HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
            if( hLanguageCombo )
            {
              int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
              if( sel != CB_ERR )
              {
                size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
                if( langIndex < ctx->languages.size() )
                {
                  ctx->tempSettings.language = ctx->languages[langIndex].EnglishName;
                  if( GetSystemLanguage() == ctx->languages[langIndex].EnglishName )
                    ctx->tempSettings.language = L"";
                }
              }
            }

            HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
            int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
            if( sel != CB_ERR )
            {
              size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
              if( voiceIndex < ctx->voices.size() )
              {
                ctx->tempSettings.voice = ctx->voices[voiceIndex].key;
              }
            }

            int volumeValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_VOLUME_SLIDER, TBM_GETPOS, 0, 0 );
            ctx->tempSettings.volume = CLAMPED_VOICE_VOLUME( volumeValue );

            int rateValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_RATE_SLIDER, TBM_GETPOS, 0, 0 );
            ctx->tempSettings.rate = CLAMPED_VOICE_RATE( rateValue );
            ctx->tempSettings.speakDirectlyWhenClickingPhrase = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.rememberCategoryWindowSize = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.autoresizeCategoryWindow = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.minimizeCategoryWindowAutomatically = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.increaseVolumeWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.reduceOtherAudioWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.stopPreviousPlayback = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.showTouchKeyboard = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.showQuickButtons = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            GetSelectedOutputDevice( hDlg, ctx, ctx->tempSettings.outputDeviceId, ctx->tempSettings.outputDeviceName );

            ctx->accepted = true;
            EndDialog( hDlg, IDOK );
            return TRUE;
          }

          case IDCANCEL:
            EndDialog( hDlg, IDCANCEL );
            return TRUE;

          case IDC_SETTINGS_LISTEN_SETTINGS:
            // Classic Sound control panel, Recording tab: where "Listen to this device"
            // lives. Windows offers apps no supported way to change it, so the user does.
            ShellExecute( hDlg, L"open", L"control.exe", L"mmsys.cpl,,1", NULL, SW_SHOWNORMAL );
            return TRUE;

          case IDC_SETTINGS_HELP:
            // In the language picked in the dialog, so it matches its text (SET-F17).
            OpenHelpPage( GetSelectedLanguageForLocalization( hDlg, ctx ) );
            return TRUE;
        }
        break;
      }

      case WM_HELP:
        // F1 while the modal dialog is open (the main window's accelerator can't see it).
        OpenHelpPage( GetSelectedLanguageForLocalization( hDlg, ctx ) );
        return TRUE;

      case WM_HSCROLL:
      {
        HWND hCtrl = (HWND) lParam;
        if( hCtrl == GetDlgItem( hDlg, IDC_SETTINGS_VOLUME_SLIDER ) )
        {
          SyncSliderToEdit( hDlg, IDC_SETTINGS_VOLUME_SLIDER, IDC_SETTINGS_VOLUME_EDIT, FALSE );
          return TRUE;
        }
        if( hCtrl == GetDlgItem( hDlg, IDC_SETTINGS_RATE_SLIDER ) )
        {
          SyncSliderToEdit( hDlg, IDC_SETTINGS_RATE_SLIDER, IDC_SETTINGS_RATE_EDIT, TRUE );
          return TRUE;
        }
        break;
      }

      case WM_CTLCOLORLISTBOX:
      {
        HDC hdcStatic = (HDC) wParam;
        if( ctx->noVoiceForLanguage )
        {
          COMBOBOXINFO info;
          info.cbSize = sizeof( info );
          SendMessage( GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO ), CB_GETCOMBOBOXINFO, 0, (LPARAM) &info );
          if( info.hwndList == ( (HWND) lParam ) )
          {
            SetTextColor( hdcStatic, RGB( 255, 0, 0 ) );
            SetBkMode( hdcStatic, TRANSPARENT );
          }
        }
        return (INT_PTR) GetStockObject( WHITE_BRUSH );
      }


      case WM_CTLCOLOREDIT:
      {
        HDC hdcStatic = (HDC) wParam;
        if( GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO ) == (HWND) lParam && ctx->noVoiceForLanguage )
        {
          SetTextColor( hdcStatic, RGB( 255, 0, 0 ) );
          SetBkMode( hdcStatic, TRANSPARENT );
        }
        return (INT_PTR) GetStockObject( WHITE_BRUSH );
      }
    }
  }

  return FALSE;
}

bool ShowSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings )
{
  SettingsDialogContext context;
  context.tempSettings = settings;
  context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry( context.tempSettings.language.empty() ? GetSystemLanguage() : context.tempSettings.language );
  if( context.voices.empty() )
  {
    context.noVoiceForLanguage = true;
    context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
  }
  else
  {
    context.noVoiceForLanguage = false;
  }
  context.languages.assign( SUPPORTED_LANGUAGES.begin(), SUPPORTED_LANGUAGES.end() );

  DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_SETTINGS_DIALOG ), owner, SettingsDialogProc, (LPARAM) &context );

  if( !context.accepted )
    return false;
  settings = context.tempSettings;
  return true;
}
