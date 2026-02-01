# Contributing to SimonSays

Thanks for helping improve SimonSays! This guide explains how to propose changes and keep the project healthy.

## Ways to contribute
- Report bugs and usability issues.
- Improve documentation and localization.
- Add tests or small fixes.
- Propose new features through a focused design discussion first.

## Development setup
1. Use Visual Studio 2022 (or newer) with the Desktop development with C++ workload.
2. Ensure the Windows 10/11 SDK is installed.
3. Clone the repo and open the solution: `SimonSays.sln`.
4. Build the **Release** or **Debug** configuration for **Win32** (or your target architecture).

## Coding standards
- Follow existing code style in the repository (naming, spacing, includes).
- Prefer small, focused changes with clear intent.
- Update or add comments only when they clarify non-obvious logic.
- Keep dependencies minimal; avoid adding new ones unless required.

## Testing
- Build successfully before opening a PR.
- Run and verify the core user flows you touched (text-to-speech, phrase selection, sound markers).
- Add or update tests if applicable.

## Issues
- Search existing issues before filing new ones.
- When reporting: include OS version, locale, relevant voice settings, reproduction steps, and expected vs. actual behavior.

## Pull requests
- Create a feature branch from `main`.
- Keep commits small and meaningful; prefer descriptive commit messages.
- Include a short summary of changes and testing done in the PR description.
- Reference related issues (e.g., `Fixes #123`).
- Be responsive to review feedback; keep discussions concise and technical.

## Localization and phrases
- For new or updated phrase sets, include language/locale details and verify registry defaults where applicable.
- Ensure fallback behavior remains safe when voices are unavailable.

## Security
- Do not disclose potential vulnerabilities publicly; email the maintainer or open a private security report if available.

## License
By contributing, you agree that your contributions will be licensed under the project’s existing license (OSL-3.0).
