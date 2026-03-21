#ifndef LOCALIZED_STRINGS_H
#define LOCALIZED_STRINGS_H

#include "stdafx.h"

static const std::vector<std::pair<int, const wchar_t *>> DEFAULT_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Play>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Settings" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Default text:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insert default text automatically" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Language:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voice:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Test Voice" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Rate (-10 to 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Speak phrase immediately when selecting" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Remember category window size" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimize category window automatically" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Increase SimonSays volume when playing" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Temporarily reduce other audio when playing" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Click to show" },
  { TRAYICON_SHOW_ID, L"Show" },
  { TRAYICON_HIDE_ID, L"Hide" },
  { TRAYICON_SETTINGS_ID, L"Settings" },
  { TRAYICON_ABOUT_ID, L"About" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Exit" },
  { ABOUT_TITTLE_ID, L"About SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simply Speak\n\nVersion " },
  { ABOUT_2_ID, L"\n\nA simple accessibility tool that \nlets you turn text into speech instantly.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays Error" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays currently only supports bottom taskbar" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Change category name" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Edit phrase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Current:" },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"A category with this name already exists. Please choose a different name." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Category Name Conflict" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Delete Category" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Delete category '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' and all its phrases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Delete Phrase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Delete phrase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Add new category" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Add new phrase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Category name:" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Phrase text:" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"New default phrase" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Import Categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Overwrite Category '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Overwrite Category?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Categories imported successfully." },
  { IMPORT_SUCCESS_TITLE_ID, L"Import Successful" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Failed to import categories." },
  { IMPORT_FAILURE_TITLE_ID, L"Import Failed" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Export Categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"You can export all categories or just the selected one.\nDo you want to export only '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' category?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Export Selection" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Categories exported successfully." },
  { EXPORT_SUCCESS_TITLE_ID, L"Export Successful" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Failed to export categories." },
  { EXPORT_FAILURE_TITLE_ID, L"Export Failed" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Help - F2: Settings" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Help Guide

## Overview
SimonSays is a Windows tray-based text-to-speech (TTS) assistant, created by Juan Rey Saura, optimized for quick speech, phrase libraries, and inline sound playback. It supports multiple languages, customizable voices, and user-friendly features for efficient communication.

## Shortcuts
- `F1`: open Help.
- `F2`: open Settings.
- `F3`: add after current selection (category/phrase context).
- `F4`: edit current selection.
- `F5` / `F6`: move to previous / next selection in lists.
- `F8`: delete current selection.
- `F9`: import categories.
- `F10`: export categories.
- `Play>` button: start playback (click).

## Launching and basic usage
1. Start SimonSays; it appears in the Windows taskbar tray.
2. Left-click the tray icon or choose `Show` to open the main window.
3. Type custom text in the input box, or click a saved phrase in the Categories window.
4. Press the `Play>` button (or hotkey if configured) to speak.
5. While playing, the button shows `>` to indicate active playback.

## Categories window (phrases)
- Open via `Categories` button. Resize/remember size via Settings.
- Select a category on the top and phrases on the bottom.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled; otherwise, it will load the phrase into the main input box without speaking.

### Managing categories
- Add: select category → (shortcut: `F3`) `Add category` dialog → enter name → `OK`.
- Rename: select category → (shortcut: `F4`) `Edit` → change name → `OK`.
- Move selection: `F5`/`F6` to move to previous/next item when browsing lists.
- Delete: select category → (shortcut: `F8`) `Delete`; confirm deletion of category and its phrases.

### Managing phrases
- Add: select phrase → (shortcut: `F3`) `Add phrase` → enter text → `OK`.
- Edit: select phrase → (shortcut: `F4`) `Edit` → update text → `OK`.
- Move selection: `F5`/`F6` to move to previous/next phrase.
- Delete: select phrase → (shortcut: `F8`) `Delete`; confirm.

## Settings (F2)
Open Settings (`F2`) to control:
- **Default text**: toggle `Insert default text automatically` and customize default phrase.
- **Language labels**: choose UI language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voice**: pick installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Remember category window size`.
  - `Minimize category window automatically` after selecting.
  - `Increase SimonSays volume when playing`.
  - `Temporarily reduce other audio when playing`.
- `Test Voice`: audition the selected voice.
- `OK` saves changes; `Cancel` discards.

## Tray icon menu
- `Show` / `Hide` main UI.
- `Settings`.
- `About` shows version and credits.
- `Web` opens project page if configured.
- `Exit` quits SimonSays.

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Mixing speech and sounds
- Use delimiter `♫` (SOUND_NOTE_DELIMITER) to embed audio filenames in text:
  - Example: `Hello ♫notification.wav♫, please wait.`
  - Text outside delimiters is spoken; text inside is treated as a sound file and played with `PlaySound`.
- Supported audio: `.wav`, `.mid`, `.midi`.
- Use absolute or relative paths accessible to the process. Pair delimiters to avoid trailing text being treated as audio.

## Importing and exporting categories
- Export: choose to export all categories or only the selected one when prompted. Success/failure messages appear per language (shortcut: `F10`).
- Import: choose a file; if a category exists, you will be asked to overwrite. Success/failure messages appear per language (shortcut: `F9`).

## Defaults storage
- Default phrases are created per language under registry key `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` on first run.
- To reinstall defaults for a language: close SimonSays → delete the language key → restart SimonSays.

## Taskbar and window behavior
- SimonSays currently supports bottom taskbar alignment.
- Category window can auto-minimize or remember size based on Settings.

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language not applied**: reopen Settings and confirm language selection; ensure localization strings exist for the chosen language.

## About
- `About` dialog lists version, description, and copyright.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> SPANISH_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insertar texto predeterminado automáticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volumen (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidad (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Hablar frase al seleccionarla" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recordar tamaño de la ventana de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar ventana de categorías automáticamente" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar volumen de SimonSays al reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente otros sonidos al reproducir" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Haz clic para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configuración" },
  { TRAYICON_ABOUT_ID, L"Acerca de" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Salir" },
  { ABOUT_TITTLE_ID, L"Acerca de SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplemente Habla\n\nVersión " },
  { ABOUT_2_ID, L"\n\nUna herramienta de accesibilidad sencilla que\nconvierte texto en voz al instante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays solo admite la barra de tareas inferior actualmente" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambiar nombre de categoría" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ya existe una categoría con este nombre. Por favor, elige un nombre diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicto de nombres de categoría" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoría" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"¿Eliminar categoría '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' y todas sus frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"¿Eliminar frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Agregar nueva categoría" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Agregar nueva frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nombre de categoría :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto de frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nueva frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorías" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"¿Sobrescribir la categoría '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"¿Sobrescribir categoría?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas correctamente." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importación exitosa" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No se pudieron importar las frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error al importar" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorías" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Puedes exportar todas las categorías o solo la seleccionada.\n¿Quieres exportar solo la categoría '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selección" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas correctamente." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportación exitosa" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No se pudieron exportar las frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error al exportar" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ayuda - F2: Configuración" },
  { HELP_CONTENT_ID, LR"HELP(# Guía de Ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para habla rápida, bibliotecas de frases y reproducción de sonidos en línea. Soporta varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

## Atajos
- `F1`: abrir Ayuda.
- `F2`: abrir Configuración.
- `F3`: añadir después de la selección actual (contexto categoría/frase).
- `F4`: editar selección actual.
- `F5` / `F6`: mover a la selección anterior / siguiente en listas.
- `F8`: eliminar selección actual.
- `F9`: importar categorías.
- `F10`: exportar categorías.
- Botón `Play>`: iniciar reproducción (clic).

## Inicio y uso básico
1. Inicia SimonSays; aparece en la bandeja de la barra de tareas de Windows.
2. Haz clic izquierdo en el icono de la bandeja o elige `Show` para abrir la ventana principal.
3. Escribe texto personalizado en la caja de entrada o haz clic en una frase guardada en la ventana de Categorías.
4. Pulsa el botón `Play>` (o atajo si está configurado) para hablar.
5. Durante la reproducción, el botón muestra `>` para indicar estado activo.

## Ventana de Categorías (frases)
- Ábrela con el botón `Categories`. Puedes redimensionarla/recordar tamaño desde Configuración.
- Selecciona una categoría arriba y frases abajo.
- Un clic en una frase la reproduce automáticamente si `Speak phrase immediately when selecting` está activado; si no, carga la frase en la caja principal sin hablar.

### Gestionar categorías
- Añadir: selecciona categoría → (atajo: `F3`) diálogo `Add category` → escribe nombre → `OK`.
- Renombrar: selecciona categoría → (atajo: `F4`) `Edit` → cambia nombre → `OK`.
- Mover selección: `F5`/`F6` para mover al elemento anterior/siguiente.
- Eliminar: selecciona categoría → (atajo: `F8`) `Delete`; confirma eliminación de categoría y frases.

### Gestionar frases
- Añadir: selecciona frase → (atajo: `F3`) `Add phrase` → escribe texto → `OK`.
- Editar: selecciona frase → (atajo: `F4`) `Edit` → actualiza texto → `OK`.
- Mover selección: `F5`/`F6` para mover a la frase anterior/siguiente.
- Eliminar: selecciona frase → (atajo: `F8`) `Delete`; confirma.

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Insert default text automatically` y personalizar frase por defecto.
- **Idioma de la interfaz**: elegir idioma de UI.
- **Voz**: seleccionar voz SAPI instalada según preferencia.
- **Volumen/Velocidad**: ajustar `Volume (10-100)` y `Rate (-10 to 10)`.
- **Comportamiento de reproducción**:
  - `Speak phrase immediately when selecting`.
  - `Remember category window size`.
  - `Minimize category window automatically`.
  - `Increase SimonSays volume when playing`.
  - `Temporarily reduce other audio when playing`.
- `Test Voice`: probar voz.
- `OK` guarda cambios; `Cancel` descarta.

## Menú del icono de bandeja
- `Show` / `Hide` interfaz principal.
- `Settings`.
- `About` muestra versión y créditos.
- `Web` abre la página del proyecto (si está configurada).
- `Exit` cierra SimonSays.

Puedes mover la ventana emergente de bandeja y otros diálogos arrastrando la barra de título. Redimensiona la ventana de Categorías arrastrando los bordes y activa `Remember category window size` para conservar tamaño.

## Mezclar voz y sonidos
- Usa el delimitador `♫` (`SOUND_NOTE_DELIMITER`) para insertar nombres de archivo de audio en el texto:
  - Ejemplo: `Hello ♫notification.wav♫, please wait.`
  - El texto fuera de delimitadores se habla; el texto dentro se trata como archivo de sonido con `PlaySound`.
- Audio soportado: `.wav`, `.mid`, `.midi`.
- Usa rutas absolutas o relativas accesibles al proceso. Empareja delimitadores para evitar interpretar texto final como audio.

## Importar y exportar categorías
- Exportar: puedes exportar todas las categorías o solo la seleccionada (atajo: `F10`).
- Importar: elige archivo; si una categoría existe, se pedirá sobrescribir (atajo: `F9`).

## Almacenamiento de valores por defecto
- Las frases por defecto se crean por idioma en `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` al primer inicio.
- Para reinstalar valores por defecto de un idioma: cierra SimonSays → elimina la clave del idioma → reinicia SimonSays.

## Comportamiento de barra de tareas y ventana
- SimonSays soporta actualmente barra de tareas inferior.
- La ventana de Categorías puede minimizarse automáticamente o recordar tamaño según Configuración.

## Solución de problemas
- **Fallo de inicialización de voz**: verifica SAPI y voces compatibles.
- **No se reproducen sonidos**: confirma ruta/extensión y delimitadores `♫`.
- **Errores de importación/exportación**: revisa permisos/ruta de disco y reintenta.
- **Idioma UI no aplicado**: reabre Configuración y confirma idioma.

## Acerca de
- El diálogo `About` muestra versión, descripción y copyright.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> ARABIC_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"تشغيل>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"الفئات" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"الإعدادات" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"النص الافتراضي:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"إدراج النص الافتراضي تلقائياً" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"اللغة:" },
  { SETTINGS_VOICE_LABEL_ID, L"الصوت:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"اختبار الصوت" },
  { SETTINGS_VOLUME_LABEL_ID, L"مستوى الصوت (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"السرعة (-10 إلى 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"نطق العبارة مباشرة عند التحديد" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"تذكر حجم نافذة الفئات" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"تصغير نافذة الفئات تلقائياً" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"زيادة مستوى صوت SimonSays أثناء التشغيل" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"خفض الأصوات الأخرى مؤقتاً أثناء التشغيل" },
  { SETTINGS_OK_BUTTON_ID, L"موافق" },
  { SETTINGS_CANCEL_BUTTON_ID, L"إلغاء" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - انقر للإظهار" },
  { TRAYICON_SHOW_ID, L"إظهار" },
  { TRAYICON_HIDE_ID, L"إخفاء" },
  { TRAYICON_SETTINGS_ID, L"الإعدادات" },
  { TRAYICON_ABOUT_ID, L"حول" },
  { TRAYICON_WEB_ID, L"الموقع" },
  { TRAYICON_EXIT_ID, L"خروج" },
  { ABOUT_TITTLE_ID, L"حول SimonSays" },
  { ABOUT_1_ID, L"SimonSays - فقط تحدث\n\nالإصدار " },
  { ABOUT_2_ID, L"\n\nأداة وصول بسيطة لتحويل النص إلى كلام فوراً.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"خطأ في SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"يدعم SimonSays حالياً شريط المهام السفلي فقط" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"تغيير اسم الفئة" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"تحرير العبارة" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"الحالي : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"موافق" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"إلغاء" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"يوجد بالفعل فئة بهذا الاسم. الرجاء اختيار اسم مختلف." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"تعارض أسماء الفئات" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"حذف الفئة" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"هل تريد حذف الفئة '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' وجميع عباراتك؟" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"حذف العبارة" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"هل تريد حذف العبارة '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'؟" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"إضافة فئة جديدة" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"إضافة عبارة جديدة" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"اسم الفئة :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"نص العبارة :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"موافق" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"إلغاء" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"عبارة افتراضية جديدة" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"استيراد الفئات" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"الكتابة فوق الفئة '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'؟" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"الكتابة فوق الفئة؟" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"تم استيراد العبارات بنجاح." },
  { IMPORT_SUCCESS_TITLE_ID, L"تم الاستيراد بنجاح" },
  { IMPORT_FAILURE_MESSAGE_ID, L"فشل استيراد العبارات." },
  { IMPORT_FAILURE_TITLE_ID, L"فشل الاستيراد" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"تصدير الفئات" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"يمكنك تصدير كل الفئات أو الفئة المحددة فقط.\nهل تريد تصدير الفئة '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' فقط؟" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"تصدير التحديد" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"تم تصدير العبارات بنجاح." },
  { EXPORT_SUCCESS_TITLE_ID, L"تم التصدير بنجاح" },
  { EXPORT_FAILURE_MESSAGE_ID, L"فشل تصدير العبارات." },
  { EXPORT_FAILURE_TITLE_ID, L"فشل التصدير" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: المساعدة - F2: الإعدادات" },
  { HELP_CONTENT_ID, LR"HELP(# دليل SimonSays

## نظرة عامة
SimonSays هو مساعد تحويل النص إلى كلام (TTS) لنظام Windows، أنشأه Juan Rey Saura، ومُحسّن للتحدث السريع ومكتبات العبارات وتشغيل الأصوات داخل النص.

## الاختصارات
- `F1`: فتح المساعدة
- `F2`: فتح الإعدادات
- `F3`: إضافة بعد التحديد الحالي
- `F4`: تعديل التحديد الحالي
- `F5` / `F6`: نقل إلى السابق / التالي
- `F8`: حذف التحديد الحالي
- `F9`: استيراد الفئات
- `F10`: تصدير الفئات
- `Play>`: بدء التشغيل

## الاستخدام الأساسي
1. شغّل SimonSays (يظهر في شريط المهام).
2. انقر على أيقونة الشريط واختر `Show`.
3. اكتب نصًا أو اختر عبارة محفوظة.
4. اضغط `Play>`.

## الفئات والعبارات
- افتح عبر زر `Categories`.
- النقر مرة واحدة على العبارة: تشغيل فوري إذا كان الخيار مفعّلًا، وإلا تُحمّل فقط في مربع الإدخال.
- إدارة العناصر: إضافة (`F3`)، تعديل (`F4`)، نقل (`F5`/`F6`)، حذف (`F8`).

## الإعدادات (F2)
يمكنك ضبط لغة الواجهة، صوت SAPI، النص الافتراضي، مستوى الصوت/السرعة، وسلوك التشغيل.

## قائمة الأيقونة
`Show` / `Hide`، `Settings`، `About`، `Web`، `Exit`.

## الكلام + الأصوات
استخدم `♫` (`SOUND_NOTE_DELIMITER`) لإدراج ملفات صوت داخل النص.
الصيغ المدعومة: `.wav`، `.mid`، `.midi`.

## استيراد / تصدير
- استيراد: `F9`
- تصدير: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> BASQUE_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Erreproduzitu>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategoriak" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Ezarpenak" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testu lehenetsia:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Txertatu testu lehenetsia automatikoki" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Hizkuntza:" },
  { SETTINGS_VOICE_LABEL_ID, L"Ahotsa:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Ahotsa probatu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Bolumena (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Abiadura (-10etik 10era):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Hautatzean esaldia berehala esan" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Gogoratu kategoriaren leihoaren tamaina" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizatu kategoriaren leihoa automatikoki" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Handitu SimonSays bolumena erreproduzitzean" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Murriztu aldi baterako beste audioak erreproduzitzean" },
  { SETTINGS_OK_BUTTON_ID, L"Ados" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Utzi" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Egin klik erakusteko" },
  { TRAYICON_SHOW_ID, L"Erakutsi" },
  { TRAYICON_HIDE_ID, L"Ezkutatu" },
  { TRAYICON_SETTINGS_ID, L"Ezarpenak" },
  { TRAYICON_ABOUT_ID, L"Honi buruz" },
  { TRAYICON_WEB_ID, L"Webgunea" },
  { TRAYICON_EXIT_ID, L"Irten" },
  { ABOUT_TITTLE_ID, L"SimonSays-ri buruz" },
  { ABOUT_1_ID, L"SimonSays - Hitz egin besterik ez\n\nBertsioa " },
  { ABOUT_2_ID, L"\n\nTestua berehala ahots bihurtzen duen\nirisgarritasun tresna sinplea.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays errorea" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays-ek unean behetik dagoen ataza-barra soilik onartzen du" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Kategoriaren izena aldatu" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Esaldia editatu" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Orain : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Ados" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Utzi" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Izen honekin kategoria bat dagoeneko existitzen da. Mesedez, aukeratu izen desberdin bat." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Kategoriaren izen gatazka" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Kategoria ezabatu" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' eta bere esaldi guztiak ezabatu?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Esaldia ezabatu" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Esaldia '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' ezabatu?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Kategoria berria gehitu" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Esaldi berria gehitu" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Kategoriaren izena :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Esaldiaren testua :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Ados" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Utzi" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Esaldi lehenetsi berria" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategoriak inportatu" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Gainidatzi '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' kategoria?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Gainidatzi kategoria?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Esaldiak ongi inportatu dira." },
  { IMPORT_SUCCESS_TITLE_ID, L"Inportazio arrakastatsua" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Esaldien inportazioak huts egin du." },
  { IMPORT_FAILURE_TITLE_ID, L"Inportazioak huts egin du" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategoriak esportatu" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategoria guztiak edo hautatutakoa soilik esporta ditzakezu.\nHautatutako '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' kategoria soilik esportatu nahi duzu?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Esportatu hautapena" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Esaldiak ongi esportatu dira." },
  { EXPORT_SUCCESS_TITLE_ID, L"Esportazio arrakastatsua" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Esaldien esportazioak huts egin du." },
  { EXPORT_FAILURE_TITLE_ID, L"Esportazioak huts egin du" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Laguntza - F2: Ezarpenak" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Laguntza

## Ikuspegi orokorra
SimonSays Windows-eko testutik hizketara (TTS) laguntzailea da, Juan Rey Saurak sortua, hizketa azkarra, esaldi-liburutegiak eta lerro barruko soinuak erreproduzitzeko optimizatua.

## Laster-teklak
- `F1`: Laguntza ireki
- `F2`: Ezarpenak ireki
- `F3`: Uneko hautapenaren ondoren gehitu
- `F4`: Uneko hautapena editatu
- `F5` / `F6`: Aurreko / hurrengo hautapenera mugitu
- `F8`: Uneko hautapena ezabatu
- `F9`: Kategoriak inportatu
- `F10`: Kategoriak esportatu
- `Play>`: Erreprodukzioa hasi

## Oinarrizko erabilera
1. Abiarazi SimonSays (ataza-barrako erretiluan agertzen da).
2. Egin klik ikonoan eta aukeratu `Show`.
3. Idatzi testua edo aukeratu gordetako esaldia.
4. Sakatu `Play>`.

## Kategoriak eta esaldiak
- Ireki `Categories` bidez.
- Esaldi batean klik bakarra: berehala hitz egin (aukera aktibatuta badago), bestela kargatu soilik.
- Ekintzak: gehitu (`F3`), editatu (`F4`), mugitu (`F5`/`F6`), ezabatu (`F8`).

## Ezarpenak (F2)
UI hizkuntza, SAPI ahotsa, testu lehenetsia, bolumena/abiadura eta erreprodukzio-portaera konfiguratu.

## Erretiluko menua
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## Hizketa + soinuak
Erabili `♫` (`SOUND_NOTE_DELIMITER`) testuan audio fitxategiak txertatzeko.
Onartutako formatuak: `.wav`, `.mid`, `.midi`.

## Inportatu / Esportatu
- Inportatu: `F9`
- Esportatu: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> CATALAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova la veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament en seleccionar-la" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Feu clic per mostrar" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Amaga" },
  { TRAYICON_SETTINGS_ID, L"Configuració" },
  { TRAYICON_ABOUT_ID, L"Quant a" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Sortir" },
  { ABOUT_TITTLE_ID, L"Quant a SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parla\n\nVersió " },
  { ABOUT_2_ID, L"\n\nUna eina d'accessibilitat senzilla que\net permet convertir text en veu a l'instant.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays només admet la barra de tasques inferior actualment" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Canvia el nom de la categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Edita la frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ja existeix una categoria amb este nom. Si us plau, tria un nom diferent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicte de noms de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' i totes les seues frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Afegeix una nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Afegeix una nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de la categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Text de la frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobreescriure la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobreescriure categoria?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importades correctament." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importació correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No s'han pogut importar les frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error d'importació" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Pots exportar totes les categories o només la seleccionada.\nVols exportar només la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selecció" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportades correctament." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportació correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No s'han pogut exportar les frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error d'exportació" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configuració" },
  { HELP_CONTENT_ID, LR"HELP(# Guia d'Ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteques de frases i reproducció de sons en línia.

## Dreceres de teclat
- `F1`: obrir Ajuda
- `F2`: obrir Configuració
- `F3`: afegir després de la selecció actual
- `F4`: editar la selecció actual
- `F5` / `F6`: moure a l'anterior / següent
- `F8`: eliminar la selecció actual
- `F9`: importar categories
- `F10`: exportar categories
- `Play>`: iniciar reproducció

## Ús bàsic
1. Inicia SimonSays (icona a la safata del sistema).
2. Fes clic a la icona i tria `Show`.
3. Escriu text o selecciona una frase desada.
4. Prem `Play>`.

## Categories i frases
- Obre amb `Categories`.
- Clic simple en una frase: parla immediata si està activat; si no, només carrega la frase.
- Accions: afegir (`F3`), editar (`F4`), moure (`F5`/`F6`), eliminar (`F8`).

## Configuració (F2)
Configura idioma de la UI, veu SAPI, text per defecte, volum/velocitat i comportament de reproducció.

## Menú de safata
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## Veu + sons
Utilitza `♫` (`SOUND_NOTE_DELIMITER`) per inserir fitxers d'àudio al text.
Formats admesos: `.wav`, `.mid`, `.midi`.

## Importar / Exportar
- Importar: `F9`
- Exportar: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"播放>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"类别" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"设置" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"默认文本：" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"自动插入默认文本" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"语言：" },
  { SETTINGS_VOICE_LABEL_ID, L"语音：" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"测试语音" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100)：" },
  { SETTINGS_RATE_LABEL_ID, L"语速 (-10 到 10)：" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"选择短语时立即朗读" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"记住分类窗口大小" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"自动最小化分类窗口" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"播放时提高 SimonSays 音量" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"播放时临时降低其他音频" },
  { SETTINGS_OK_BUTTON_ID, L"确定" },
  { SETTINGS_CANCEL_BUTTON_ID, L"取消" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - 点击显示" },
  { TRAYICON_SHOW_ID, L"显示" },
  { TRAYICON_HIDE_ID, L"隐藏" },
  { TRAYICON_SETTINGS_ID, L"设置" },
  { TRAYICON_ABOUT_ID, L"关于" },
  { TRAYICON_WEB_ID, L"网站" },
  { TRAYICON_EXIT_ID, L"退出" },
  { ABOUT_TITTLE_ID, L"关于 SimonSays" },
  { ABOUT_1_ID, L"SimonSays - 简单开口说\n\n版本 " },
  { ABOUT_2_ID, L"\n\n一款简单的辅助工具，可立即将文本转换为语音。\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays 错误" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays 目前仅支持底部任务栏" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"更改类别名称" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"编辑短语" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"当前 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"确定" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"取消" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"已存在一个具有此名称的类别。请选择一个不同的名称。" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"类别名称冲突" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"删除类别" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"删除类别 '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 和它的所有短语吗？" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"删除短语" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"删除短语 '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'？" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"添加新类别" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"添加新短语" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"类别名称:" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"短语文本:" },
  { ADD_DIALOG_OK_BUTTON_ID, L"确定" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"取消" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"新的默认短语" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"导入类别" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"覆盖类别 '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"覆盖类别?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"短语导入成功。" },
  { IMPORT_SUCCESS_TITLE_ID, L"导入成功" },
  { IMPORT_FAILURE_MESSAGE_ID, L"导入短语失败。" },
  { IMPORT_FAILURE_TITLE_ID, L"导入失败" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"导出类别" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"你可以导出所有类别或只导出选中的类别。\n你要仅导出 '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 类别吗?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"导出选择" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"短语导出成功。" },
  { EXPORT_SUCCESS_TITLE_ID, L"导出成功" },
  { EXPORT_FAILURE_MESSAGE_ID, L"导出短语失败。" },
  { EXPORT_FAILURE_TITLE_ID, L"导出失败" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1：帮助 - F2：设置" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays 帮助

## 概述
SimonSays 是由 Juan Rey Saura 创建的 Windows 文本转语音（TTS）助手，专注于快速发声、短语库和内联声音播放。

## 快捷键
- `F1`：打开帮助
- `F2`：打开设置
- `F3`：在当前选择后添加
- `F4`：编辑当前选择
- `F5` / `F6`：移动到上一个 / 下一个
- `F8`：删除当前选择
- `F9`：导入类别
- `F10`：导出类别
- `Play>`：开始播放

## 基本使用
1. 启动 SimonSays（显示在任务栏托盘）。
2. 点击托盘图标并选择 `Show`。
3. 输入文本或选择保存的短语。
4. 点击 `Play>` 按钮。

## 类别与短语
- 通过 `Categories` 打开。
- 单击短语时：若启用即刻朗读则直接播放，否则仅加载到输入框。
- 支持添加(`F3`)、编辑(`F4`)、移动(`F5`/`F6`)、删除(`F8`)。

## 设置 (F2)
可配置界面语言、SAPI 语音、默认文本、音量/语速和播放行为。

## 托盘菜单
`Show` / `Hide`、`Settings`、`About`、`Web`、`Exit`。

## 语音 + 声音
使用 `♫`（`SOUND_NOTE_DELIMITER`）在文本中嵌入音频文件。
支持格式：`.wav`、`.mid`、`.midi`。

## 导入 / 导出
- 导入：`F9`
- 导出：`F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> FRENCH_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Lire>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Catégories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Paramètres" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texte par défaut :" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insérer automatiquement le texte par défaut" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Langue :" },
  { SETTINGS_VOICE_LABEL_ID, L"Voix :" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Tester la voix" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100) :" },
  { SETTINGS_RATE_LABEL_ID, L"Débit (-10 à 10) :" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Dire immédiatement la phrase lors de la sélection" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Mémoriser la taille de la fenêtre des catégories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Réduire automatiquement la fenêtre des catégories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenter le volume de SimonSays pendant la lecture" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Réduire temporairement les autres sons pendant la lecture" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annuler" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Cliquez pour afficher" },
  { TRAYICON_SHOW_ID, L"Afficher" },
  { TRAYICON_HIDE_ID, L"Masquer" },
  { TRAYICON_SETTINGS_ID, L"Paramètres" },
  { TRAYICON_ABOUT_ID, L"À propos" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Quitter" },
  { ABOUT_TITTLE_ID, L"À propos de SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parler\n\nVersion " },
  { ABOUT_2_ID, L"\n\nUn outil d'accessibilité simple qui\ntransforme instantanément du texte en voix.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erreur SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays prend actuellement uniquement en charge la barre des tâches inférieure" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Changer le nom de la catégorie" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Modifier la phrase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actuel : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Annuler" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Une catégorie avec ce nom existe déjà. Veuillez choisir un nom différent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflit de noms de catégorie" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Supprimer la catégorie" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Supprimer la catégorie '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' et toutes ses phrases ?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Supprimer la phrase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Supprimer la phrase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' ?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Ajouter une nouvelle catégorie" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Ajouter une nouvelle phrase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de catégorie :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texte de la phrase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Annuler" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nouvelle phrase par défaut" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importer des catégories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Remplacer la catégorie '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' ?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Remplacer la catégorie ?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Expressions importées avec succès." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importation réussie" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Échec de l'importation des phrases." },
  { IMPORT_FAILURE_TITLE_ID, L"Échec de l'importation" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exporter des catégories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Vous pouvez exporter toutes les catégories ou seulement celle sélectionnée.\nVoulez-vous exporter uniquement la catégorie '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' ?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exporter la sélection" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Expressions exportées avec succès." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportation réussie" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Échec de l'exportation des phrases." },
  { EXPORT_FAILURE_TITLE_ID, L"Échec de l'exportation" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1 : Aide - F2 : Paramètres" },
  { HELP_CONTENT_ID, LR"HELP(# Guide d'aide SimonSays

## Vue d'ensemble
SimonSays est un assistant Windows de synthèse vocale (TTS), créé par Juan Rey Saura, optimisé pour la parole rapide, les bibliothèques de phrases et la lecture de sons intégrés. Il prend en charge plusieurs langues, des voix personnalisables et des fonctionnalités pratiques pour une communication efficace.

## Raccourcis
- `F1` : ouvrir l'aide.
- `F2` : ouvrir les paramètres.
- `F3` : ajouter après la sélection actuelle (catégorie/phrase).
- `F4` : modifier la sélection actuelle.
- `F5` / `F6` : déplacer vers la sélection précédente / suivante.
- `F8` : supprimer la sélection actuelle.
- `F9` : importer des catégories.
- `F10` : exporter des catégories.
- Bouton `Play>` : démarrer la lecture (clic).

## Démarrage et utilisation de base
1. Lancez SimonSays ; il apparaît dans la zone de notification Windows.
2. Cliquez gauche sur l’icône de la zone de notification ou choisissez `Show` pour ouvrir la fenêtre principale.
3. Saisissez un texte personnalisé ou cliquez sur une phrase enregistrée dans la fenêtre Catégories.
4. Appuyez sur `Play>` pour parler.
5. Pendant la lecture, le bouton affiche `>`.

## Fenêtre Catégories (phrases)
- Ouvrez via le bouton `Categories`. Redimensionnement/mémorisation via Paramètres.
- Sélectionnez une catégorie en haut et les phrases en bas.
- Un clic sur une phrase lance la lecture immédiate si `Speak phrase immediately when selecting` est activé ; sinon la phrase est chargée sans lecture.

### Gérer les catégories
- Ajouter : sélectionner catégorie → (`F3`) `Add category` → nom → `OK`.
- Renommer : sélectionner catégorie → (`F4`) `Edit` → modifier → `OK`.
- Déplacer : `F5`/`F6` pour précédent/suivant.
- Supprimer : sélectionner catégorie → (`F8`) `Delete` ; confirmer.

### Gérer les phrases
- Ajouter : sélectionner phrase → (`F3`) `Add phrase` → texte → `OK`.
- Modifier : sélectionner phrase → (`F4`) `Edit` → mise à jour → `OK`.
- Déplacer : `F5`/`F6`.
- Supprimer : sélectionner phrase → (`F8`) `Delete` ; confirmer.

## Paramètres (F2)
Ouvrez les paramètres (`F2`) pour contrôler : texte par défaut, langue de l’interface, voix SAPI, volume/vitesse et comportement de lecture.
`Test Voice` permet d’écouter la voix. `OK` enregistre, `Cancel` annule.

## Menu de l’icône de notification
- `Show` / `Hide`
- `Settings`
- `About`
- `Web`
- `Exit`

Vous pouvez déplacer les fenêtres en glissant la barre de titre. Redimensionnez la fenêtre Catégories et activez `Remember category window size` pour conserver la taille.

## Mélange parole + sons
- Utilisez le délimiteur `♫` (`SOUND_NOTE_DELIMITER`) :
  - Exemple : `Hello ♫notification.wav♫, please wait.`
- En dehors des délimiteurs : texte parlé.
- Entre délimiteurs : fichier audio joué via `PlaySound`.
- Formats : `.wav`, `.mid`, `.midi`.

## Importer et exporter des catégories
- Export : toutes les catégories ou seulement la sélection (`F10`).
- Import : choisir un fichier ; confirmation d’écrasement si nécessaire (`F9`).

## Stockage des valeurs par défaut
- Valeurs créées sous `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` au premier lancement.
- Pour réinstaller : fermer SimonSays → supprimer la clé langue → relancer.

## Dépannage
- **Voix indisponible** : vérifier SAPI et voix installées.
- **Son non lu** : vérifier chemin/extension et délimiteurs `♫`.
- **Erreur import/export** : vérifier permissions et emplacement.

## À propos
- `About` affiche version, description et copyright.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> GALICIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir automaticamente o texto predeterminado" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase inmediatamente ao seleccionala" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamaño da xanela de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automáticamente a xanela de categorías" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume de SimonSays ao reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente outros sons ao reproducir" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Fai clic para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configuración" },
  { TRAYICON_ABOUT_ID, L"Sobre" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Saír" },
  { ABOUT_TITTLE_ID, L"Sobre SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplemente fala\n\nVersión " },
  { ABOUT_2_ID, L"\n\nUnha ferramenta de accesibilidade sinxela que\npermite converter texto en voz ao instante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erro de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays só admite actualmente a barra de tarefas inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambiar o nome da categoría" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar a frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Xa existe unha categoría con este nome. Por favor, elixe un nome diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflito de nomes de categoría" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoría" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Eliminar a categoría '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e todas as súas frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Eliminar a frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Engadir nova categoría" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Engadir nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome da categoría :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto da frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorías" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobrescribir a categoría '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobrescribir categoría?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas correctamente." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importación correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Fallou a importación das frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Erro na importación" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorías" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Podes exportar todas as categorías ou só a seleccionada.\nQueres exportar só a categoría '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selección" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas correctamente." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportación correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Fallou a exportación das frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Erro na exportación" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Axuda - F2: Configuración" },
  { HELP_CONTENT_ID, LR"HELP(# Guía de Axuda de SimonSays

## Visión xeral
SimonSays é un asistente TTS para Windows, creado por Juan Rey Saura, optimizado para fala rápida, bibliotecas de frases e reprodución de sons en liña.

## Atallos
- `F1`: abrir Axuda
- `F2`: abrir Configuración
- `F3`: engadir despois da selección actual
- `F4`: editar selección actual
- `F5` / `F6`: mover á anterior / seguinte
- `F8`: eliminar selección actual
- `F9`: importar categorías
- `F10`: exportar categorías
- `Play>`: iniciar reprodución

## Uso básico
1. Inicia SimonSays (na bandexa do sistema).
2. Preme na icona e escolle `Show`.
3. Escribe texto ou selecciona unha frase gardada.
4. Preme `Play>`.

## Categorías e frases
- Abrir con `Categories`.
- Clic simple: fala inmediata se está activado; se non, só carga a frase.
- Accións: engadir (`F3`), editar (`F4`), mover (`F5`/`F6`), eliminar (`F8`).

## Configuración (F2)
Configura idioma da interface, voz SAPI, texto predeterminado, volume/velocidade e comportamento de reprodución.

## Menú da bandexa
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## Fala + sons
Usa `♫` (`SOUND_NOTE_DELIMITER`) para inserir ficheiros de audio no texto.
Formatos compatibles: `.wav`, `.mid`, `.midi`.

## Importar / Exportar
- Importar: `F9`
- Exportar: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> GERMAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Abspielen>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategorien" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Einstellungen" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Standardtext:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Standardtext automatisch einfügen" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Sprache:" },
  { SETTINGS_VOICE_LABEL_ID, L"Stimme:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Stimme testen" },
  { SETTINGS_VOLUME_LABEL_ID, L"Lautstärke (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Geschwindigkeit (-10 bis 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Satz bei Auswahl sofort aussprechen" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Größe des Kategorienfensters merken" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Kategorienfenster automatisch minimieren" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Lautstärke von SimonSays beim Abspielen erhöhen" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Andere Audios beim Abspielen vorübergehend reduzieren" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Abbrechen" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Klicken zum Anzeigen" },
  { TRAYICON_SHOW_ID, L"Anzeigen" },
  { TRAYICON_HIDE_ID, L"Ausblenden" },
  { TRAYICON_SETTINGS_ID, L"Einstellungen" },
  { TRAYICON_ABOUT_ID, L"Über" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Beenden" },
  { ABOUT_TITTLE_ID, L"Über SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Einfach sprechen\n\nVersion " },
  { ABOUT_2_ID, L"\n\nEin einfaches Barrierefreiheits-Tool,\nmit dem Sie Text sofort in Sprache umwandeln können.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays-Fehler" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays unterstützt derzeit nur die untere Taskleiste" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Kategoriename ändern" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Satz bearbeiten" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Aktuell : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Abbrechen" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Eine Kategorie mit diesem Namen existiert bereits. Bitte wählen Sie einen anderen Namen." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Kategoriename Konflikt" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Kategorie löschen" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategorie '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' und alle ihre Sätze löschen?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Satz löschen" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Satz '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' löschen?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Neue Kategorie hinzufügen" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Neuen Satz hinzufügen" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Kategoriename :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Satztext :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Abbrechen" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Neuer Standardsatz" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategorien importieren" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Kategorie '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' überschreiben?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Kategorie überschreiben?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Sätze erfolgreich importiert." },
  { IMPORT_SUCCESS_TITLE_ID, L"Import erfolgreich" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Sätze konnten nicht importiert werden." },
  { IMPORT_FAILURE_TITLE_ID, L"Import fehlgeschlagen" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategorien exportieren" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Sie können alle Kategorien oder nur die ausgewählte exportieren.\nMöchten Sie nur die Kategorie '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' exportieren?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Auswahl exportieren" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Sätze erfolgreich exportiert." },
  { EXPORT_SUCCESS_TITLE_ID, L"Export erfolgreich" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Sätze konnten nicht exportiert werden." },
  { EXPORT_FAILURE_TITLE_ID, L"Export fehlgeschlagen" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Hilfe - F2: Einstellungen" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Hilfe

## Übersicht
SimonSays ist ein Windows-Text-zu-Sprache-Assistent (TTS), erstellt von Juan Rey Saura, optimiert für schnelle Sprache, Phrasenbibliotheken und eingebettete Soundwiedergabe.

## Tastenkürzel
- `F1`: Hilfe öffnen
- `F2`: Einstellungen öffnen
- `F3`: Nach aktueller Auswahl hinzufügen
- `F4`: Aktuelle Auswahl bearbeiten
- `F5` / `F6`: Zur vorherigen / nächsten Auswahl
- `F8`: Aktuelle Auswahl löschen
- `F9`: Kategorien importieren
- `F10`: Kategorien exportieren
- `Play>`: Wiedergabe starten

## Grundbedienung
1. SimonSays starten (Taskleistenbereich).
2. Über Tray-Symbol `Show` wählen.
3. Text eingeben oder gespeicherte Phrase wählen.
4. `Play>` drücken.
5. Während der Wiedergabe zeigt der Button `>`.

## Kategorienfenster
- Mit `Categories` öffnen.
- Kategorie oben, Phrasen unten.
- Einfachklick spricht sofort (wenn aktiviert), sonst nur übernehmen.

## Kategorien verwalten
- Hinzufügen: `F3`
- Umbenennen: `F4`
- Verschieben: `F5`/`F6`
- Löschen: `F8`

## Phrasen verwalten
- Hinzufügen: `F3`
- Bearbeiten: `F4`
- Verschieben: `F5`/`F6`
- Löschen: `F8`

## Einstellungen (F2)
Sprache, Stimme, Standardtext, Lautstärke/Geschwindigkeit und Wiedergabeverhalten konfigurieren.

## Tray-Menü
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`.

## Sprache + Sounds
Verwenden Sie `♫` (`SOUND_NOTE_DELIMITER`) für Audiodateien im Text.
Unterstützt: `.wav`, `.mid`, `.midi`.

## Import/Export
- Import: `F9`
- Export: `F10`

## Standardspeicher
`HKCU\SOFTWARE\SimonSays\Phrases\<Language>`.

## Fehlerbehebung
SAPI/Voices prüfen, Dateipfade prüfen, Berechtigungen prüfen.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> HEBREW_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"נגן>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"קטגוריות" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"הגדרות" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"טקסט ברירת מחדל:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"הכנס טקסט ברירת מחדל אוטומטית" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"שפה:" },
  { SETTINGS_VOICE_LABEL_ID, L"קול:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"בדוק קול" },
  { SETTINGS_VOLUME_LABEL_ID, L"עוצמה (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"מהירות (-10 עד 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"אמור את המשפט מיד בעת הבחירה" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"זכור את גודל חלון הקטגוריות" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"מזער את חלון הקטגוריות אוטומטית" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"הגדל את עוצמת SimonSays בזמן הנגינה" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"הנמך זמנית צלילים אחרים בזמן הנגינה" },
  { SETTINGS_OK_BUTTON_ID, L"אישור" },
  { SETTINGS_CANCEL_BUTTON_ID, L"ביטול" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - לחץ כדי להציג" },
  { TRAYICON_SHOW_ID, L"הצג" },
  { TRAYICON_HIDE_ID, L"הסתר" },
  { TRAYICON_SETTINGS_ID, L"הגדרות" },
  { TRAYICON_ABOUT_ID, L"אודות" },
  { TRAYICON_WEB_ID, L"אתר" },
  { TRAYICON_EXIT_ID, L"יציאה" },
  { ABOUT_TITTLE_ID, L"אודות SimonSays" },
  { ABOUT_1_ID, L"SimonSays - פשוט לדבר\n\nגרסה " },
  { ABOUT_2_ID, L"\n\nכלי נגישות פשוט שהופך טקסט לדיבור מיד.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"שגיאת SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays תומך כרגע רק בשורת משימות תחתונה" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"שנה שם קטגוריה" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"ערוך משפט" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"נוכחי : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"אישור" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"ביטול" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"כבר קיימת קטגוריה עם שם זה. אנא בחר שם אחר." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"ניגוד שמות קטגוריות" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"הסר קטגוריה" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"האם להסיר את הקטגוריה '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' ואת כל המשפטים שלה?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"הסר משפט" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"האם להסיר את המשפט '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"הוסף קטגוריה חדשה" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"הוסף משפט חדש" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"שם קטגוריה :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"טקסט המשפט :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"אישור" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"ביטול" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"משפט ברירת מחדל חדש" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"ייבוא קטגוריות" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"לשכתב את הקטגוריה '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"לשכתב קטגוריה?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"המשפטים יובאו בהצלחה." },
  { IMPORT_SUCCESS_TITLE_ID, L"ייבוא הצליח" },
  { IMPORT_FAILURE_MESSAGE_ID, L"כשל בייבוא המשפטים." },
  { IMPORT_FAILURE_TITLE_ID, L"ייבוא נכשל" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"ייצוא קטגוריות" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"תוכל לייצא את כל הקטגוריות או רק את הנבחרת.\nהאם לייצא רק את הקטגוריה '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"ייצוא בחירה" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"המשפטים יוצאו בהצלחה." },
  { EXPORT_SUCCESS_TITLE_ID, L"ייצוא הצליח" },
  { EXPORT_FAILURE_MESSAGE_ID, L"כשל בייצוא המשפטים." },
  { EXPORT_FAILURE_TITLE_ID, L"ייצוא נכשל" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: עזרה - F2: הגדרות" },
  { HELP_CONTENT_ID, LR"HELP(# עזרה עבור SimonSays

## סקירה
SimonSays הוא עוזר המרת טקסט לדיבור (TTS) עבור Windows, נוצר על ידי Juan Rey Saura, ומותאם לדיבור מהיר, ספריות משפטים והשמעת צלילים בתוך הטקסט.

## קיצורי מקלדת
- `F1`: פתיחת עזרה
- `F2`: פתיחת הגדרות
- `F3`: הוספה אחרי הבחירה הנוכחית
- `F4`: עריכת הבחירה הנוכחית
- `F5` / `F6`: מעבר לקודם / הבא
- `F8`: מחיקת הבחירה הנוכחית
- `F9`: ייבוא קטגוריות
- `F10`: ייצוא קטגוריות
- `Play>`: התחלת השמעה

## שימוש בסיסי
1. הפעל את SimonSays (מופיע במגש המערכת).
2. לחץ על האייקון ובחר `Show`.
3. הקלד טקסט או בחר משפט שמור.
4. לחץ `Play>`.

## קטגוריות ומשפטים
- פתיחה דרך `Categories`.
- לחיצה בודדת על משפט: השמעה מיידית אם האפשרות פעילה, אחרת טעינה בלבד לשדה הטקסט.
- פעולות: הוספה (`F3`), עריכה (`F4`), הזזה (`F5`/`F6`), מחיקה (`F8`).

## הגדרות (F2)
הגדרת שפת ממשק, קול SAPI, טקסט ברירת מחדל, עוצמה/מהירות והתנהגות השמעה.

## תפריט מגש
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## דיבור + צלילים
השתמש ב-`♫` (`SOUND_NOTE_DELIMITER`) כדי לשלב קבצי שמע בתוך הטקסט.
פורמטים נתמכים: `.wav`, `.mid`, `.midi`.

## ייבוא / ייצוא
- ייבוא: `F9`
- ייצוא: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> HINDI_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"चलाएँ>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"श्रेणियाँ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"सेटिंग्स" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"डिफ़ॉल्ट पाठ:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"डिफ़ॉल्ट पाठ स्वतः जोड़ें" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"भाषा:" },
  { SETTINGS_VOICE_LABEL_ID, L"आवाज़:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"आवाज़ परीक्षण" },
  { SETTINGS_VOLUME_LABEL_ID, L"वॉल्यूम (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"गति (-10 से 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"चयन करते ही वाक्य बोलें" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"श्रेणी विंडो का आकार याद रखें" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"श्रेणी विंडो को स्वतः छोटा करें" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"चलाते समय SimonSays की आवाज़ बढ़ाएँ" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"चलाते समय अन्य ऑडियो अस्थायी रूप से घटाएँ" },
  { SETTINGS_OK_BUTTON_ID, L"ठीक" },
  { SETTINGS_CANCEL_BUTTON_ID, L"रद्द करें" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - दिखाने के लिए क्लिक करें" },
  { TRAYICON_SHOW_ID, L"दिखाएँ" },
  { TRAYICON_HIDE_ID, L"छिपाएँ" },
  { TRAYICON_SETTINGS_ID, L"सेटिंग्स" },
  { TRAYICON_ABOUT_ID, L"परिचय" },
  { TRAYICON_WEB_ID, L"वेब" },
  { TRAYICON_EXIT_ID, L"बाहर निकलें" },
  { ABOUT_TITTLE_ID, L"SimonSays के बारे में" },
  { ABOUT_1_ID, L"SimonSays - बस बोलें\n\nसंस्करण " },
  { ABOUT_2_ID, L"\n\nएक सरल एक्सेसिबिलिटी टूल\nजो तुरंत पाठ को भाषण में बदलता है।\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays त्रुटि" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays वर्तमान में केवल निचली टास्कबार का समर्थन करता है" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"श्रेणी का नाम बदलें" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"वाक्य संपादित करें" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"वर्तमान : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"ठीक" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"रद्द करें" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"इस नाम की एक श्रेणी पहले से मौजूद है। कृपया एक अलग नाम चुनें।" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"श्रेणी नाम संघर्ष" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"श्रेणी हटाएं" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"क्या आप श्रेणी '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' और इसके सभी वाक्य हटाना चाहते हैं?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"वाक्य हटाएं" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"क्या आप वाक्य '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' हटाना चाहते हैं?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"नई श्रेणी जोड़ें" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"नया वाक्य जोड़ें" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"श्रेणी का नाम :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"वाक्य का पाठ :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"ठीक" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"रद्द करें" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"नया डिफ़ॉल्ट वाक्य" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"श्रेणियाँ आयात करें" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"श्रेणी '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' अधिलेखित करें?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"श्रेणी अधिलेखित करें?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"वाक्य सफलतापूर्वक आयात किए गए।" },
  { IMPORT_SUCCESS_TITLE_ID, L"आयात सफल" },
  { IMPORT_FAILURE_MESSAGE_ID, L"वाक्य आयात करने में विफल रहा।" },
  { IMPORT_FAILURE_TITLE_ID, L"आयात असफल" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"श्रेणियाँ निर्यात करें" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"आप सभी श्रेणियाँ या केवल चयनित श्रेणी निर्यात कर सकते हैं।\nक्या आप केवल श्रेणी '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' निर्यात करना चाहते हैं?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"चयन निर्यात करें" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"वाक्य सफलतापूर्वक निर्यात किए गए।" },
  { EXPORT_SUCCESS_TITLE_ID, L"निर्यात सफल" },
  { EXPORT_FAILURE_MESSAGE_ID, L"वाक्य निर्यात करने में विफल रहा।" },
  { EXPORT_FAILURE_TITLE_ID, L"निर्यात असफल" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: सहायता - F2: सेटिंग्स" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays सहायता मार्गदर्शिका

## अवलोकन
SimonSays एक Windows टेक्स्ट-टू-स्पीच (TTS) सहायक है, जिसे Juan Rey Saura ने बनाया है। यह तेज़ बोलने, वाक्य लाइब्रेरी और इनलाइन साउंड प्लेबैक के लिए अनुकूलित है।

## शॉर्टकट
- `F1`: सहायता खोलें
- `F2`: सेटिंग्स खोलें
- `F3`: वर्तमान चयन के बाद जोड़ें
- `F4`: वर्तमान चयन संपादित करें
- `F5` / `F6`: पिछले / अगले चयन पर जाएँ
- `F8`: वर्तमान चयन हटाएँ
- `F9`: श्रेणियाँ आयात करें
- `F10`: श्रेणियाँ निर्यात करें
- `Play>`: प्लेबैक शुरू करें

## मूल उपयोग
1. SimonSays शुरू करें (टास्कबार ट्रे में दिखेगा)।
2. ट्रे आइकन पर क्लिक करें और `Show` चुनें।
3. टेक्स्ट लिखें या सहेजा हुआ वाक्य चुनें।
4. `Play>` दबाएँ।

## श्रेणियाँ और वाक्य
- `Categories` से खोलें।
- एक बार क्लिक: विकल्प चालू होने पर तुरंत बोलेगा, नहीं तो केवल लोड करेगा।
- क्रियाएँ: जोड़ें (`F3`), संपादित करें (`F4`), स्थानांतरित करें (`F5`/`F6`), हटाएँ (`F8`)।

## सेटिंग्स (F2)
UI भाषा, SAPI आवाज़, डिफ़ॉल्ट टेक्स्ट, वॉल्यूम/गति और प्लेबैक व्यवहार सेट करें।

## ट्रे मेन्यू
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## आवाज़ + ध्वनियाँ
`♫` (`SOUND_NOTE_DELIMITER`) का उपयोग करके टेक्स्ट में ऑडियो फ़ाइल जोड़ें।
समर्थित प्रारूप: `.wav`, `.mid`, `.midi`.

## आयात / निर्यात
- आयात: `F9`
- निर्यात: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> ITALIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Riproduci>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorie" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Impostazioni" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testo predefinito:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserisci automaticamente il testo predefinito" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Lingua:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voce:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova voce" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocità (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Pronuncia subito la frase quando la selezioni" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Ricorda la dimensione della finestra delle categorie" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Riduci automaticamente la finestra delle categorie" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumenta il volume di SimonSays durante la riproduzione" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Riduci temporaneamente gli altri audio durante la riproduzione" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annulla" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Clic per mostrare" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Nascondi" },
  { TRAYICON_SETTINGS_ID, L"Impostazioni" },
  { TRAYICON_ABOUT_ID, L"Informazioni" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Esci" },
  { ABOUT_TITTLE_ID, L"Informazioni su SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Parla semplicemente\n\nVersione " },
  { ABOUT_2_ID, L"\n\nUno strumento di accessibilità semplice\nche trasforma il testo in voce all'istante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Errore di SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays attualmente supporta solo la barra delle applicazioni inferiore" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambia nome categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Modifica frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Attuale : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Annulla" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Esiste già una categoria con questo nome. Per favore, scegli un nome diverso." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflitto nomi categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Elimina categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Eliminare la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e tutte le sue frasi?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Elimina frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Eliminare la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Aggiungi nuova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Aggiungi nuova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Testo della frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Annulla" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nuova frase predefinita" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importa categorie" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sovrascrivere la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sovrascrivere la categoria?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frasi importate con successo." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importazione riuscita" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Impossibile importare le frasi." },
  { IMPORT_FAILURE_TITLE_ID, L"Importazione non riuscita" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Esporta categorie" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Puoi esportare tutte le categorie o solo quella selezionata.\nVuoi esportare solo la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Esporta selezione" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frasi esportate con successo." },
  { EXPORT_SUCCESS_TITLE_ID, L"Esportazione riuscita" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Impossibile esportare le frasi." },
  { EXPORT_FAILURE_TITLE_ID, L"Esportazione non riuscita" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Aiuto - F2: Impostazioni" },
  { HELP_CONTENT_ID, LR"HELP(# Guida di SimonSays

## Panoramica
SimonSays è un assistente Windows di sintesi vocale (TTS), creato da Juan Rey Saura, ottimizzato per comunicazione rapida, librerie di frasi e suoni in linea.

## Scorciatoie
- `F1`: apri Aiuto
- `F2`: apri Impostazioni
- `F3`: aggiungi dopo la selezione corrente
- `F4`: modifica selezione corrente
- `F5` / `F6`: sposta alla selezione precedente / successiva
- `F8`: elimina selezione corrente
- `F9`: importa categorie
- `F10`: esporta categorie
- `Play>`: avvia riproduzione

## Avvio e uso base
1. Avvia SimonSays dalla tray di Windows.
2. Clic sull’icona e seleziona `Show`.
3. Scrivi testo o seleziona una frase salvata.
4. Premi `Play>`.

## Finestra Categorie
- Apri con `Categories`.
- Categoria in alto, frasi in basso.
- Clic singolo: parla subito se opzione attiva, altrimenti carica solo il testo.

## Gestione categorie e frasi
- Aggiungi (`F3`), modifica (`F4`), sposta (`F5`/`F6`), elimina (`F8`).

## Impostazioni (F2)
Configura testo predefinito, lingua UI, voce SAPI, volume/velocità, comportamento playback.

## Menu tray
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`.

## Voce + suoni
Usa `♫` (`SOUND_NOTE_DELIMITER`) per inserire file audio nel testo.
Formati: `.wav`, `.mid`, `.midi`.

## Import/Export
- Importa categorie: `F9`
- Esporta categorie: `F10`

## Registro
Valori predefiniti in `HKCU\SOFTWARE\SimonSays\Phrases\<Language>`.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> JAPANESE_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"再生>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"カテゴリ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"設定" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"既定のテキスト:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"既定のテキストを自動挿入" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"言語:" },
  { SETTINGS_VOICE_LABEL_ID, L"音声:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"音声をテスト" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"速度 (-10 ～ 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"選択したらすぐにフレーズを話す" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウのサイズを記憶" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウを自動的に最小化" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"再生中に SimonSays の音量を上げる" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"再生中に他の音声を一時的に下げる" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"キャンセル" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - クリックして表示" },
  { TRAYICON_SHOW_ID, L"表示" },
  { TRAYICON_HIDE_ID, L"非表示" },
  { TRAYICON_SETTINGS_ID, L"設定" },
  { TRAYICON_ABOUT_ID, L"バージョン情報" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"終了" },
  { ABOUT_TITTLE_ID, L"SimonSays について" },
  { ABOUT_1_ID, L"SimonSays - ただ話すだけ\n\nバージョン " },
  { ABOUT_2_ID, L"\n\nテキストをすぐに音声に変換できる\nシンプルなアクセシビリティ ツールです。\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays エラー" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays は現在、下部タスクバーのみをサポートしています" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"カテゴリ名を変更" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"フレーズを編集" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"現在 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"キャンセル" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"この名前のカテゴリはすでに存在します。別の名前を選択してください。" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"カテゴリ名の競合" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"カテゴリを削除" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"カテゴリ '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' とそのすべてのフレーズを削除しますか？" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"フレーズを削除" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"フレーズ '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' を削除しますか？" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"新しいカテゴリを追加" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"新しいフレーズを追加" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"カテゴリ名 :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"フレーズのテキスト :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"キャンセル" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"新しい既定のフレーズ" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"カテゴリをインポート" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"カテゴリ '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' を上書きしますか？" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"カテゴリを上書きしますか?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"フレーズを正常にインポートしました。" },
  { IMPORT_SUCCESS_TITLE_ID, L"インポート成功" },
  { IMPORT_FAILURE_MESSAGE_ID, L"フレーズのインポートに失敗しました。" },
  { IMPORT_FAILURE_TITLE_ID, L"インポート失敗" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"カテゴリをエクスポート" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"すべてのカテゴリをエクスポートするか、選択したものだけをエクスポートできます。\n選択したカテゴリ『" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"』だけをエクスポートしますか？" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"選択をエクスポート" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"フレーズを正常にエクスポートしました。" },
  { EXPORT_SUCCESS_TITLE_ID, L"エクスポート成功" },
  { EXPORT_FAILURE_MESSAGE_ID, L"フレーズのエクスポートに失敗しました。" },
  { EXPORT_FAILURE_TITLE_ID, L"エクスポート失敗" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: ヘルプ - F2: 設定" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays ヘルプ

## 概要
SimonSays は、Juan Rey Saura が作成した Windows 用テキスト読み上げ（TTS）アシスタントです。素早い発話、フレーズ管理、インライン音声再生に最適化されています。

## ショートカット
- `F1`: ヘルプを開く
- `F2`: 設定を開く
- `F3`: 現在の選択の後に追加
- `F4`: 現在の選択を編集
- `F5` / `F6`: 前 / 次へ移動
- `F8`: 現在の選択を削除
- `F9`: カテゴリをインポート
- `F10`: カテゴリをエクスポート
- `Play>`: 再生開始

## 基本操作
1. SimonSays を起動（タスクトレイに表示）。
2. トレイアイコンから `Show` を選択。
3. テキスト入力または保存済みフレーズを選択。
4. `Play>` を押して読み上げ。

## カテゴリとフレーズ
- `Categories` で開く。
- フレーズをクリックすると、設定に応じて即時再生または入力欄へ挿入。
- 追加 (`F3`)、編集 (`F4`)、移動 (`F5`/`F6`)、削除 (`F8`)。

## 設定 (F2)
UI 言語、SAPI 音声、既定テキスト、音量/速度、再生動作を設定。

## トレイメニュー
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`。

## 音声 + サウンド
`♫`（`SOUND_NOTE_DELIMITER`）で音声ファイルを埋め込み。
対応形式: `.wav`, `.mid`, `.midi`。

## インポート / エクスポート
- インポート: `F9`
- エクスポート: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> KOREAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"재생>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"카테고리" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"설정" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"기본 텍스트:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"기본 텍스트 자동 삽입" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"언어:" },
  { SETTINGS_VOICE_LABEL_ID, L"음성:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"음성 테스트" },
  { SETTINGS_VOLUME_LABEL_ID, L"볼륨 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"속도 (-10~10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"선택 시 문장을 바로 말하기" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"카테고리 창 크기 기억" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"카테고리 창 자동 최소화" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"재생 중 SimonSays 볼륨 증가" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"재생 중 다른 오디오 임시 감소" },
  { SETTINGS_OK_BUTTON_ID, L"확인" },
  { SETTINGS_CANCEL_BUTTON_ID, L"취소" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - 클릭하여 표시" },
  { TRAYICON_SHOW_ID, L"표시" },
  { TRAYICON_HIDE_ID, L"숨기기" },
  { TRAYICON_SETTINGS_ID, L"설정" },
  { TRAYICON_ABOUT_ID, L"정보" },
  { TRAYICON_WEB_ID, L"웹" },
  { TRAYICON_EXIT_ID, L"종료" },
  { ABOUT_TITTLE_ID, L"SimonSays 정보" },
  { ABOUT_1_ID, L"SimonSays - 그냥 말하세요\n\n버전 " },
  { ABOUT_2_ID, L"\n\n텍스트를 즉시 음성으로 변환하는\n간단한 접근성 도구입니다.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays 오류" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays는 현재 하단 작업 표시줄만 지원합니다" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"카테고리 이름 변경" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"문구 편집" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"현재 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"확인" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"취소" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"이 이름의 카테고리가 이미 존재합니다. 다른 이름을 선택하세요." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"카테고리 이름 충돌" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"카테고리 삭제" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"' 카테고리를 삭제하시겠습니까?" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 및 모든 문구를 삭제하시겠습니까?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"문구 삭제" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"' 문구를 삭제하시겠습니까?" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"새 카테고리 추가" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"새 문구 추가" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"카테고리 이름 :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"문구 텍스트 :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"확인" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"취소" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"새 기본 문구" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"카테고리 가져오기" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"카테고리 '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'를 덮어쓸까요?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"카테고리 덮어쓰기?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"문구를 성공적으로 가져왔습니다." },
  { IMPORT_SUCCESS_TITLE_ID, L"가져오기 성공" },
  { IMPORT_FAILURE_MESSAGE_ID, L"문구를 가져오지 못했습니다." },
  { IMPORT_FAILURE_TITLE_ID, L"가져오기 실패" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"카테고리 내보내기" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"모든 카테고리를 내보내거나 선택한 것만 내보낼 수 있습니다.\n선택한 '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 카테고리만 내보내겠습니까?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"선택 항목 내보내기" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"문구를 성공적으로 내보냈습니다." },
  { EXPORT_SUCCESS_TITLE_ID, L"내보내기 성공" },
  { EXPORT_FAILURE_MESSAGE_ID, L"문구를 내보내지 못했습니다." },
  { EXPORT_FAILURE_TITLE_ID, L"내보내기 실패" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: 도움말 - F2: 설정" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays 도움말

## 개요
SimonSays는 Juan Rey Saura가 만든 Windows용 TTS(텍스트 음성 변환) 도우미로, 빠른 발화, 문구 라이브러리, 인라인 사운드 재생에 최적화되어 있습니다.

## 단축키
- `F1`: 도움말 열기
- `F2`: 설정 열기
- `F3`: 현재 선택 다음에 추가
- `F4`: 현재 선택 편집
- `F5` / `F6`: 이전 / 다음으로 이동
- `F8`: 현재 선택 삭제
- `F9`: 카테고리 가져오기
- `F10`: 카테고리 내보내기
- `Play>`: 재생 시작

## 기본 사용법
1. SimonSays 실행(작업 표시줄 트레이에 표시).
2. 트레이 아이콘에서 `Show` 선택.
3. 텍스트 입력 또는 저장된 문구 선택.
4. `Play>` 버튼 클릭.

## 카테고리/문구
- `Categories` 버튼으로 열기.
- 문구 단일 클릭 시 설정에 따라 즉시 읽기 또는 입력창에 로드.
- 추가(`F3`), 편집(`F4`), 이동(`F5`/`F6`), 삭제(`F8`).

## 설정 (F2)
UI 언어, SAPI 음성, 기본 텍스트, 볼륨/속도, 재생 동작 설정.

## 트레이 메뉴
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`.

## 음성 + 사운드
`♫` (`SOUND_NOTE_DELIMITER`)로 오디오 파일 삽입.
지원 형식: `.wav`, `.mid`, `.midi`.

## 가져오기 / 내보내기
- 가져오기: `F9`
- 내보내기: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> PORTUGUESE_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproduzir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorias" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configurações" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto padrão:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir texto padrão automaticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Testar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase imediatamente ao selecionar" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamanho da janela de categorias" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automaticamente a janela de categorias" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume do SimonSays ao reproduzir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reduzir temporariamente outros áudios ao reproduzir" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Clique para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configurações" },
  { TRAYICON_ABOUT_ID, L"Sobre" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Sair" },
  { ABOUT_TITTLE_ID, L"Sobre o SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplesmente fale\n\nVersão " },
  { ABOUT_2_ID, L"\n\nUma ferramenta de acessibilidade simples\nque transforma texto em voz instantaneamente.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erro do SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"O SimonSays atualmente suporta apenas a barra de tarefas inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Alterar nome da categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Atual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Já existe uma categoria com esse nome. Por favor, escolha um nome diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflito de nomes de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Excluir categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Excluir a categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e todas as suas frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Excluir frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Excluir a frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Adicionar nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Adicionar nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome da categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto da frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase padrão" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorias" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobrescrever a categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobrescrever categoria?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas com sucesso." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importação bem-sucedida" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Falha ao importar as frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Falha na importação" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorias" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Você pode exportar todas as categorias ou apenas a selecionada.\nDeseja exportar apenas a categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar seleção" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas com sucesso." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportação bem-sucedida" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Falha ao exportar as frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Falha na exportação" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configurações" },
  { HELP_CONTENT_ID, LR"HELP(# Guia de Ajuda do SimonSays

## Visão geral
SimonSays é um assistente TTS para Windows, criado por Juan Rey Saura, otimizado para fala rápida, biblioteca de frases e reprodução de sons em linha.

## Atalhos
- `F1`: abrir Ajuda
- `F2`: abrir Configurações
- `F3`: adicionar após seleção atual
- `F4`: editar seleção atual
- `F5` / `F6`: mover para anterior / próximo
- `F8`: excluir seleção atual
- `F9`: importar categorias
- `F10`: exportar categorias
- `Play>`: iniciar reprodução

## Uso básico
1. Inicie o SimonSays (bandeja da barra de tarefas).
2. Clique no ícone e escolha `Show`.
3. Digite texto ou selecione frase salva.
4. Pressione `Play>`.

## Categorias e frases
- Abra com `Categories`.
- Clique simples fala imediatamente (se ativado), caso contrário apenas carrega a frase.
- Ações: adicionar (`F3`), editar (`F4`), mover (`F5`/`F6`), excluir (`F8`).

## Configurações (F2)
Idioma, voz SAPI, texto padrão, volume/velocidade e comportamento de reprodução.

## Menu da bandeja
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`.

## Fala + sons
Use `♫` (`SOUND_NOTE_DELIMITER`) para áudio embutido.
Formatos: `.wav`, `.mid`, `.midi`.

## Importação / Exportação
- Importar: `F9`
- Exportar: `F10`

## Registro
Padrões em `HKCU\SOFTWARE\SimonSays\Phrases\<Language>`.
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> RUSSIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Воспроизвести>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Категории" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Настройки" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Текст по умолчанию:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Автоматически вставлять текст по умолчанию" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Язык:" },
  { SETTINGS_VOICE_LABEL_ID, L"Голос:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Проверить голос" },
  { SETTINGS_VOLUME_LABEL_ID, L"Громкость (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Скорость (-10 до 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Произносить фразу сразу при выборе" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Запоминать размер окна категорий" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Автоматически сворачивать окно категорий" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Повышать громкость SimonSays при воспроизведении" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Временно снижать другие звуки при воспроизведении" },
  { SETTINGS_OK_BUTTON_ID, L"ОК" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Отмена" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Нажмите, чтобы показать" },
  { TRAYICON_SHOW_ID, L"Показать" },
  { TRAYICON_HIDE_ID, L"Скрыть" },
  { TRAYICON_SETTINGS_ID, L"Настройки" },
  { TRAYICON_ABOUT_ID, L"О программе" },
  { TRAYICON_WEB_ID, L"Веб" },
  { TRAYICON_EXIT_ID, L"Выход" },
  { ABOUT_TITTLE_ID, L"О программе SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Просто говорите\n\nВерсия " },
  { ABOUT_2_ID, L"\n\nПростой инструмент доступности,\nкоторый мгновенно превращает текст в речь.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Ошибка SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays в настоящее время поддерживает только нижнюю панель задач" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Изменить имя категории" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Редактировать фразу" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Текущий : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"ОК" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Отмена" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Категория с таким именем уже существует. Пожалуйста, выберите другое имя." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Конфликт имен категорий" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Удалить категорию" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Удалить категорию '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' и все ее фразы?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Удалить фразу" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Удалить фразу '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Добавить новую категорию" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Добавить новую фразу" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Название категории :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Текст фразы :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"ОК" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Отмена" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Новая фраза по умолчанию" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Импорт категорий" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Перезаписать категорию '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Перезаписать категорию?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Фразы успешно импортированы." },
  { IMPORT_SUCCESS_TITLE_ID, L"Импорт выполнен" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Не удалось импортировать фразы." },
  { IMPORT_FAILURE_TITLE_ID, L"Ошибка импорта" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Экспорт категорий" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Вы можете экспортировать все категории или только выбранную.\nХотите экспортировать только категорию '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Экспорт выбора" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Фразы успешно экспортированы." },
  { EXPORT_SUCCESS_TITLE_ID, L"Экспорт выполнен" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Не удалось экспортировать фразы." },
  { EXPORT_FAILURE_TITLE_ID, L"Ошибка экспорта" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Справка - F2: Настройки" },
  { HELP_CONTENT_ID, LR"HELP(# Справка SimonSays

## Обзор
SimonSays — это помощник TTS для Windows, созданный Juan Rey Saura, с быстрым озвучиванием, библиотеками фраз и встроенным воспроизведением звуков.

## Горячие клавиши
- `F1`: открыть справку
- `F2`: открыть настройки
- `F3`: добавить после текущего выбора
- `F4`: редактировать текущий выбор
- `F5` / `F6`: перемещение к предыдущему / следующему
- `F8`: удалить текущий выбор
- `F9`: импорт категорий
- `F10`: экспорт категорий
- `Play>`: начать воспроизведение

## Базовое использование
1. Запустите SimonSays (значок в трее).
2. Нажмите на значок и выберите `Show`.
3. Введите текст или выберите сохранённую фразу.
4. Нажмите `Play>`.

## Категории и фразы
- Откройте через `Categories`.
- Один клик по фразе: сразу озвучить (если включено), иначе только подставить в поле.
- Действия: добавить (`F3`), редактировать (`F4`), переместить (`F5`/`F6`), удалить (`F8`).

## Настройки (F2)
Язык интерфейса, голос SAPI, текст по умолчанию, громкость/скорость и поведение воспроизведения.

## Меню трея
`Show`/`Hide`, `Settings`, `About`, `Web`, `Exit`.

## Речь + звуки
Используйте `♫` (`SOUND_NOTE_DELIMITER`) для вставки аудио в текст.
Поддерживаемые форматы: `.wav`, `.mid`, `.midi`.

## Импорт / экспорт
- Импорт: `F9`
- Экспорт: `F10`
)HELP" },
};

static const std::vector<std::pair<int, const wchar_t *>> VALENCIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Provar veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament quan la seleciones" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Feu clic per mostrar" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Amaga" },
  { TRAYICON_SETTINGS_ID, L"Configuració" },
  { TRAYICON_ABOUT_ID, L"Quant a" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Eixir" },
  { ABOUT_TITTLE_ID, L"Quant a SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parla\n\nVersió " },
  { ABOUT_2_ID, L"\n\nUna eina d'accessibilitat senzilla que\net permet convertir text en veu a l'instant.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays només admet actualment la barra de tasques inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Canviar nom de categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ja existeix una categoria amb este nom. Per favor, trieu un nom diferent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicte de noms de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' i totes les seues frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Afig nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Afig nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de la categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Text de la frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories Backup (*.ssc)\0*.ssc\0All Files\0*.*\0" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportades correctament." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportació correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No s'han pogut exportar les frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error d'exportació" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobreescriure la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobreescriure categoria?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importades correctament." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importació correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No s'han pogut importar les frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error d'importació" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Pots exportar totes les categories o només la seleccionada.\nVols exportar només la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selecció" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configuració" },
  { HELP_CONTENT_ID, LR"HELP(# Guia d'Ajuda de SimonSays (Valencià)

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteca de frases i reproducció de sons en línia.

## Dreceres
- `F1`: obrir Ajuda
- `F2`: obrir Configuració
- `F3`: afegir després de la selecció actual
- `F4`: editar la selecció actual
- `F5` / `F6`: moure a l'anterior / següent
- `F8`: eliminar la selecció actual
- `F9`: importar categories
- `F10`: exportar categories
- `Play>`: iniciar reproducció

## Ús bàsic
1. Inicia SimonSays (apareix a la safata de la barra de tasques).
2. Fes clic a la icona i tria `Show`.
3. Escriu text o selecciona una frase guardada.
4. Prem `Play>`.

## Categories i frases
- Obri amb `Categories`.
- Clic simple: parla immediata si està activat; si no, només carrega la frase.
- Accions: afegir (`F3`), editar (`F4`), moure (`F5`/`F6`), eliminar (`F8`).

## Configuració (F2)
Idioma d'interfície, veu SAPI, text predeterminat, volum/velocitat i comportament de reproducció.

## Menú de safata
`Show` / `Hide`, `Settings`, `About`, `Web`, `Exit`.

## Veu + sons
Usa `♫` (`SOUND_NOTE_DELIMITER`) per a inserir fitxers d'àudio en el text.
Formats compatibles: `.wav`, `.mid`, `.midi`.

## Importar / Exportar
- Importar: `F9`
- Exportar: `F10`
)HELP" },
};

static const  std::vector < std::pair < std::wstring, std::vector<std::pair<int, const wchar_t *>>>> LOCALIZED_STRINGS = {
  { L"English", DEFAULT_LOCALIZED_UI_STRINGS },
  { L"Spanish", SPANISH_LOCALIZED_UI_STRINGS },
  { L"Arabic", ARABIC_LOCALIZED_UI_STRINGS },
  { L"Basque", BASQUE_LOCALIZED_UI_STRINGS },
  { L"Catalan", CATALAN_LOCALIZED_UI_STRINGS },
  { L"Chinese (Simplified)", CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS },
  { L"French", FRENCH_LOCALIZED_UI_STRINGS },
  { L"Galician", GALICIAN_LOCALIZED_UI_STRINGS },
  { L"German", GERMAN_LOCALIZED_UI_STRINGS },
  { L"Hebrew", HEBREW_LOCALIZED_UI_STRINGS },
  { L"Hindi", HINDI_LOCALIZED_UI_STRINGS },
  { L"Italian", ITALIAN_LOCALIZED_UI_STRINGS },
  { L"Japanese", JAPANESE_LOCALIZED_UI_STRINGS },
  { L"Korean", KOREAN_LOCALIZED_UI_STRINGS },
  { L"Portuguese", PORTUGUESE_LOCALIZED_UI_STRINGS },
  { L"Russian", RUSSIAN_LOCALIZED_UI_STRINGS },
  { L"Valencian", VALENCIAN_LOCALIZED_UI_STRINGS }
};

#endif // LOCALIZED_STRINGS_H
