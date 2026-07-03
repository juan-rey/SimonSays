//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by SimonSays.rc
//
// SimonSays - Simply Speak (Text-to-Speech Utility).
// Copyright 2025-2026 Juan Rey Saura
// Licensed under the Open Software License version 3.0 — see "LICENSE".
//
// Resource IDs only. Lives next to SimonSays.rc so Visual Studio's resource
// editor writes here directly (Save → no clobber). C++ code finds it via the
// `resources` entry in AdditionalIncludeDirectories.

#ifndef resource_h
#define resource_h

#ifndef IDC_STATIC
#define IDC_STATIC                                  -1
#endif

// Icons / menus / accelerators / dialogs ---------------------------------
#define IDI_SIMONSAYS                               101
#define IDR_MAINMENU                                102
#define IDD_ABOUTBOX                                103
#define IDD_SETTINGS_DIALOG                         104
#define IDR_MAINACCEL                               105
#define IDD_EDIT_DIALOG                             106
#define IDD_DWELL_DIALOG                            107

// Main-menu / accelerator commands ---------------------------------------
#define ID_FILE_EXIT                                1001
#define ID_HELP_ABOUT                               1002
#define ID_SETTINGS_OPEN                            1003
#define ID_EDIT_LAST_SELECTION                      1004
#define ID_DELETE_LAST_SELECTION                    1005
#define ID_MOVE_SELECTION_PREV                      1006
#define ID_MOVE_SELECTION_NEXT                      1007
#define ID_ADD_AFTER_SELECTION                      1008
#define ID_HELP_OPEN                                1009
#define ID_IMPORT_FILE                              1010
#define ID_EXPORT_FILE                              1011
#define ID_DWELL_OPEN                               1012

// Tray menu commands -----------------------------------------------------
#define ID_TRAY_SHOW_HIDE                           2001
#define ID_TRAY_SETTINGS                            2002
#define ID_TRAY_WEB                                 2003
#define ID_TRAY_ABOUT                               2004
#define ID_TRAY_FEEDBACK                            2005
#define ID_TRAY_EXIT                                2006
#define ID_TRAY_DWELL                               2007

// Taskbar controls -------------------------------------------------------
#define IDC_EDIT_PHRASE                             3001
#define IDC_BUTTON_PLAY                             3002
#define IDC_BUTTON_CATEGORIES                       3003

// Settings dialog --------------------------------------------------------
#define IDC_SETTINGS_DEFAULT_TEXT                   4001
#define IDC_SETTINGS_USE_DEFAULT_TEXT               4002
#define IDC_SETTINGS_VOICE_COMBO                    4003
#define IDC_SETTINGS_VOLUME_EDIT                    4004
#define IDC_SETTINGS_RATE_EDIT                      4005
#define IDC_SETTINGS_LANGUAGE_COMBO                 4006
#define IDC_SETTINGS_VOLUME_SLIDER                  4007
#define IDC_SETTINGS_RATE_SLIDER                    4008
#define IDC_SETTINGS_TEST_VOICE                     4009
#define IDC_SETTINGS_SPEAK_ON_CLICK                 4010
#define IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW       4011
#define IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW       4012
#define IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING   4013
#define IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING 4014
#define IDC_SETTINGS_LABEL_DEFAULT_TEXT             4015
#define IDC_SETTINGS_LABEL_LANGUAGE                 4016
#define IDC_SETTINGS_LABEL_VOICE                    4017
#define IDC_SETTINGS_LABEL_VOLUME                   4018
#define IDC_SETTINGS_LABEL_RATE                     4019
#define IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK         4020
#define IDC_SETTINGS_SHOW_TOUCH_KEYBOARD            4021

// Edit-Selection dialog --------------------------------------------------
#define IDC_EDIT_DIALOG_TEXT                        5001
#define IDC_EDIT_DIALOG_LABEL_TEXT                  5002

// Gaze dwell-click dialog ------------------------------------------------
#define IDC_DWELL_INTRO                             6001
#define IDC_DWELL_MODE_GROUP                        6002
#define IDC_DWELL_MODE_AUTO                         6003
#define IDC_DWELL_MODE_MOUSE                        6004
#define IDC_DWELL_MODE_HID                          6005
#define IDC_DWELL_MODE_OFF                          6006
#define IDC_DWELL_TIME_LABEL                        6007
#define IDC_DWELL_TIME_SLIDER                       6008
#define IDC_DWELL_TIME_EDIT                         6009
#define IDC_DWELL_TOLERANCE_LABEL                   6010
#define IDC_DWELL_TOLERANCE_SLIDER                  6011
#define IDC_DWELL_TOLERANCE_EDIT                    6012
#define IDC_DWELL_COOLDOWN_LABEL                    6013
#define IDC_DWELL_COOLDOWN_SLIDER                   6014
#define IDC_DWELL_COOLDOWN_EDIT                     6015
#define IDC_DWELL_COLOR_BUTTON                      6016
#define IDC_DWELL_DETECT_GROUP                      6017
#define IDC_DWELL_PROBE_LOOK_AREA                   6018
#define IDC_DWELL_PROBE_MOUSE_AREA                  6019
#define IDC_DWELL_STATUS                            6020
#define IDC_DWELL_APPLY                             6021
#define IDC_DWELL_PROBE_LOOK                        6022
#define IDC_DWELL_PROBE_MOUSE                       6023
#define IDC_DWELL_SIGNALS_GROUP                     6024
#define IDC_DWELL_SIGNAL_HID                        6025
#define IDC_DWELL_SIGNAL_TOOL                       6026
#define IDC_DWELL_SIGNAL_WEC                        6027
#define IDC_DWELL_RESET                             6028

// Next default values for new objects (the VS resource editor reads and
// bumps these when you add a new dialog / control / command in the IDE).
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC                                 1
#define _APS_NEXT_RESOURCE_VALUE                    108
#define _APS_NEXT_COMMAND_VALUE                     1013
#define _APS_NEXT_CONTROL_VALUE                     6029
#define _APS_NEXT_SYMED_VALUE                       110
#endif
#endif

#endif // resource_h
