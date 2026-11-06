# AGENTS.md

## Project

XBoing ("rayboing") is a C rewrite of the classic X11 blockout game using
[raylib](https://www.raylib.com/) (bundled as the `raylib/` submodule, currently
5.6-dev). Active code lives in `src/` and `include/`. `archive/` contains the
original X11 implementation for reference and is **not** compiled by the build.

## Build

Prerequisites: Premake 5 (`premake5.exe` in the repo root), a C toolchain, and
an initialized `raylib` submodule (`git submodule update --init --recursive`).

Generate and build on Windows:

```powershell
.\premake5.exe vs2022
MSBuild.exe xboing.sln -t:rayboing -p:Configuration=Debug
```

Or with GNU Make (Linux/macOS, or W64Devkit on Windows):

```sh
./premake5 gmake
make rayboing config=debug
```

Outputs go to `bin/Debug/` (`rayboing.exe` / `rayboing`, plus `raylib.lib`).
Run the game **from the repository root**; all asset and save paths are
relative to the current working directory (`resource/...`, `scores.txt`).

Notes:
- `xboing.sln` is a generated file; `build_files/` is gitignored except for
  `build_files/raylib.lua`, so project files are created by Premake.
- Debug builds are console apps; Release builds are WindowedApp.
- See `docs/building/` for Visual Studio/MSBuild, Make, and W64Devkit details.

## Tests

There is no unit-test suite. `tests/compile-test.sh` is a compile-only helper:
it compiles each `*.c` in the current directory with
`gcc $(pkg-config raylib --cflags) -I include -c` and reports pass/fail, so it
expects GNU tools plus a system raylib. Run it from the directory containing
the sources you want to check.

## Layout

| Path | Purpose |
| --- | --- |
| `src/rayboing_demo.c` | Entry point (`main`), 575x720 window, main game loop, CLI level argument, paddle/mouse input |
| `src/demo_gamemodes.c` | Game state machine (`MODE_INITGAME`, `MODE_PLAY`, `MODE_WIN`, `MODE_LOSE`, `MODE_CANCEL`, `MODE_EXIT`), lives, level advancement, HUD |
| `src/demo_blockloader.c` | Level file parsing, 15x9 block grid, block textures, walls, countdown timer |
| `src/demo_ball.c` | Ball physics, collisions, release/sticky/speed-up |
| `src/paddle.c` | Paddle movement, sizing, reverse flag, ball spawn point |
| `src/audio.c` | 46-sound audio system (`SoundID` enum) |
| `src/intro.c`, `src/scoreboard.c`, `src/demo_controls.c` | Intro screen, high-score screens/persistence, input helpers |
| `include/*.h` | Public API for the matching module |
| `resource/levels/*.data` | Level name line, time line, then 15 rows of 9 block characters |
| `resource/textures/`, `resource/sounds/` | Art and `.mp3` assets |
| `archive/` | Original X11 sources (reference only) |

## Conventions

- Pure C. Some code intentionally avoids compound literals / newer syntax for
  C89 compatibility; don't introduce C++ or new dependencies.
- Add public declarations to the matching header in `include/`; both `include/`
  and `src/` are on the include path. Include raylib as `<raylib.h>`.
- Match the surrounding file: newer raylib-derived modules use 4-space indent
  and PascalCase public functions; older modules use tabs and camelCase.
- Keep existing header-guard names when editing a header.
- Do not hard-code absolute paths; assets are loaded relative to the CWD.
- Game modes are driven by `GetGameMode()`/`SetGameMode()`; avoid bypassing the
  state machine when adding gameplay logic.

## Commit rules

- Every commit must list the agent as a co-author:
  `Co-authored-by: OpenCode <noreply@opencode.ai>`
- Never stage or commit changes without explicit approval from the user.
  Show the planned file list/diff and wait for a go-ahead first. The same
  applies to pushing.
- These rules may grow over time; always follow the latest version of this
  section.

## Known rough edges

- `KEY_X` is bound both to "quit game" (`demo_controls.c`) and "grow paddle"
  (`rayboing_demo.c`), so pressing X during play triggers both.
- Paddle/mouse input is handled both in `main` (`UpdatePaddleInput`) and in
  `RunPlayMode`; new input handling should go in one place.
- Root-level `audio.c` and `version.c` are legacy leftovers; only `src/**.c`
  is built.
