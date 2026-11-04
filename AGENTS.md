# XBoing (rayboing)

Raylib port of XBoing. C project built with Premake 5.

## Workflow

- Read repo docs (`docs/`) and existing code before changing anything.
- One issue per branch and per chat session. Work only on the selected issue.
- Focused changes only; no unrelated refactoring or formatting.
- Preserve existing conventions; do not touch `raylib/` submodule unless the issue requires it.
- Check `git status` before editing and preserve existing work.
- Explain suspected cause and proposed fix before implementing.
- Build after relevant changes; run applicable tests. Provide manual game-testing steps.
- Report exactly which checks passed, failed, or need me to playtest. Never claim an unperformed check passed.

## Hygiene

- Keep credentials, downloaded tools, build output, and chat exports out of commits.
- Before committing, review the diff and stage only intended files.
- Attribute yourself as an AI coauthor on every commit via a valid `Co-authored-by:` trailer, identifying the active tool/model. Use a documented AI attribution identity if present; otherwise ask me to agree on one before the first commit—do not invent an email.
- Ask before committing, pushing, or opening a pull request.

## Setup

- Clone with submodules; if `raylib/` empty, run `git submodule init && git submodule update`.
- `premake5` (or `premake5.exe`) must sit at repo root next to `premake5.lua`.

## Build (verified on this Windows dev machine)

- Prerequisites here: VS2022 Community (MSVC) present; premake5.exe was downloaded to repo root (gitignored). C++/Windows SDK tooling available.
- Generate VS solution projects: `.\premake5.exe vs2022`
- Build Debug: `& 'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe' xboing.sln -t:rayboing -p:Configuration=Debug`
- Build succeeded after fixing typo `bouncVariance` -> `bounceVariance` in `src/demo_ball.c`.
- Output executable: `bin\Debug\rayboing.exe` (Win32/Debug build).
- Run from repo root: `.\bin\Debug\rayboing.exe`
- Full developer compile may emit warnings; build passes with 0 errors.
- 'build_files/' contains Premake support code and generated project files. Do not manually edit generated files.

## Verification (needs runtime confirmation)

- Debug biuld and game launch confirmed on this Windows machine.
- Per-file compile (verified pattern from `tests/compile-test.sh`, intended to run where `*.c` lives): `gcc $(pkg-config raylib --cflags) -I include -c <file>`. Script paths may need adapting; treat as prototype until tested in this environment.
- No automated test suite; only compile checks and manual playtesting.
- Manual game testing: launch `bin/<Config>/rayboing`, exercise paddle/ball, blocks, scoring, level load, audio toggle if applicable; report observations.

## Layout

- `src/` sources, `include/` headers; `rayboing_demo.c` contains `main`.
- `raylib/` is a submodule; do not edit.
- `build_files/` generated/excluded.