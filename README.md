SimonSays – Speak With Ease

SimonSays is an innovative accessibility tool designed for individuals who are unable to speak but want to communicate effortlessly. Seamlessly integrated into your taskbar, SimonSays is always ready to convert your typed text into clear, natural-sounding speech with a single click.

In addition to text-to-speech, SimonSays lets you save and quickly access your most frequent phrases and sounds, making communication even faster and more convenient.

Whether you’re working, socializing, or need to express yourself quickly, SimonSays empowers you to communicate freely and confidently—without interrupting your workflow. With its intuitive interface, instant voice output, and customizable quick-access phrases, SimonSays ensures your voice is always heard.

## What's new in v0.2

This release adds expanded language packs (Italian, Portuguese), a programmatic installer for default phrases (`RegistryManager::InstallDefaultPhrases()`), a registry import helper (`RegistryManager::ImportRegistrySetupFile()`), and a mixed playback pipeline that supports inline sound markers via the `SOUND_NOTE_DELIMITER` (♫). See the ChangeLog for full details.

## Quick start

1. Build the solution in Visual Studio (recommended) or your preferred Windows C++ toolchain.
2. (Optional) Run `REGISTRY_SETUP.reg` or call `RegistryManager::InstallDefaultPhrases()` to populate default phrases.
3. Launch SimonSays — the app runs from the taskbar. Open the categories window, choose a phrase or type your message, then press Play.

## Usage — mixing speech and sounds

- Use the `SOUND_NOTE_DELIMITER` character (♫) to insert inline sound markers. Example:

  Hello ♫notification.wav♫, please wait.

  - Text outside markers is spoken with SAPI.
  - Text between markers is treated as an audio filename and played with `PlaySound`.

- Supported audio formats: `.wav`, `.mid`, `.midi`. Provide absolute or relative paths accessible to the running process.

## Registry defaults and installation

- Default phrase sets are stored under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>`.
- The repository includes `REGISTRY_SETUP.reg` with frequency-sorted multilingual defaults.
- Programmatic options:
  - `RegistryManager::InstallDefaultPhrases()` — writes default phrases directly into HKCU.
  - `RegistryManager::ImportRegistrySetupFile(const std::wstring &path)` — imports a .reg file via `reg import`.

## Troubleshooting

- Voice initialization fails: verify SAPI is installed and voices are available.
- Sound files not playing: confirm path and supported extension; check file permissions.
- Registry defaults not present: run `REGISTRY_SETUP.reg` or call `InstallDefaultPhrases()`; importing system-wide keys may require elevation.

## Contributing

See `CONTRIBUTING.md` for coding standards and commit guidelines. When filing issues include OS version, locale, and any voice settings.

## License

Licensed under the Open Software License (OSL) v3.0. See `LICENSE` for full terms.
