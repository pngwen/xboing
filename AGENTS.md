# XBoing (rayboing) — Agent Guide

Raylib port of XBoing, a blockout/paddle game, in C with Premake 5.
Entry point: `src/rayboing_demo.c`. The original 1996 X11 sources live in
`archive/` and are **not built**; `INSTALL` and `xboing.man` are historical.

## Assignment rules

- One issue per branch **and** per chat session. Branch from `main`.
- Focused changes only. No unrelated refactoring, reformatting, or renames.
- Match existing conventions: K&R braces, 4-space indent, `camelCase`
  functions, `PascalCase` for exported API. Older files mix in tabs — follow
  the file you are editing.
- Touch the `raylib/` submodule only when the issue requires it (it is pinned;
  do not update it casually).
- Check `git status` before editing and preserve any existing uncommitted work.
- Explain the suspected cause and the proposed fix **before** implementing.
- Build after relevant changes, run the applicable checks, and give specific
  manual gameplay steps. Report exactly what passed, failed, or is untested.
  Never claim a check passed that you did not run.
- Ask only the essential questions the repo cannot answer, grouped into one
  short batch.

## Hygiene and commits

- Never commit credentials, downloaded tools, build output, or chat exports.
- Before committing, review the diff and stage only intended files.
- Credit the active AI tool/model on every commit with a valid
  `Co-authored-by:` trailer. The only prior attribution in history is
  `Co-authored-by: OpenCode Beam (AI model assistance, no email provided).`
  (commit `1c3e8c3`); it carries no email address, so confirm the exact
  identity and address with the user before the first commit. Never invent an
  official-looking email address.
- Ask before committing, pushing, or opening a pull request.

## Setup (verified on the Windows dev machine, Nov 2026)

Prerequisites: VS2022 with the "Desktop development with C++" workload, and
`premake5.exe` at the repo root (gitignored via `*.exe`).

Verified toolchain: VS Community 17.11.35312, MSVC 14.41.34120, Windows SDK
10.0.22621 / 10.0.26100, Premake 5.0.0-beta2. Confirm the workload with:

```sh
& 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe' -products '*' -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath
```

```sh
git clone <url> --recurse-submodules
# if raylib/ is empty:
git submodule init && git submodule update
git submodule status   # expect 3f6d67c... raylib (5.5-1069-g3f6d67c6)
```

`docs/setup/getting-started.md` still shows an older fork URL; use this repo's
`origin` instead. `docs/building/*` are accurate for their toolchains.

## Build — TESTED

Run from the **repo root** (that is where `premake5.lua` and `premake5.exe` are):

```sh
.\premake5.exe vs2022
& 'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe' xboing.sln -t:rayboing -p:Configuration=Debug
```

- First build also compiles the `raylib` static lib (`bin\Debug\raylib.lib`).
- Output: `bin\Debug\rayboing.exe` (gitignored). Release uses
  `-p:Configuration=Release` → `bin\Release\`.
- Expect `C4244` float/int conversion warnings from `src/demo_ball.c`; only
  errors matter. A clean build exits 0.
- Generated files: `build_files/` (gitignored) and `xboing.sln` (tracked).
  Never hand-edit either; regenerating should leave `git status` clean.
- The MSBuild path depends on the installed VS edition — adjust if not Community.

## Run — TESTED

Run from the **repo root**; asset and save paths are relative:

```sh
.\bin\Debug\rayboing.exe
.\bin\Debug\rayboing.exe resource/levels/level02.data   # optional level override
```

- No argument shows the intro screen; Enter or Space starts the game.
- The game reads/writes `scores.txt` in the working directory at game over.
  It is untracked and must never be committed.

## VSCode on Windows — TESTED

Chosen approach: VS2022 + MSBuild, because both are already installed. No
`make` or W64Devkit is needed.

Required extension (not preinstalled on the reference machine):

```sh
code --install-extension ms-vscode.cpptools
```

It provides IntelliSense and the `cppvsdbg` debugger used by `launch.json`.

Local config lives in `.vscode/` and **all four files are gitignored**, so they
never appear in commits:

| File | Contents |
| --- | --- |
| `tasks.json` | `premake: generate (vs2022)`, `build: rayboing (Debug)` (default build task, `$msCompile` matcher), `build: rayboing (Release)`, `run: rayboing (Debug)` |
| `launch.json` | `Debug rayboing (MSVC)` and `Debug rayboing (custom level)`, both `cppvsdbg`, `cwd` = `${workspaceFolder}`, pre-launch build |
| `c_cpp_properties.json` | include paths for `include/`, `src/`, `raylib/src`, `raylib/src/external/glfw/include`; `windows-msvc-x64`; C11 |

Usage: run `premake: generate (vs2022)` once (Tasks: Run Task), then Ctrl+Shift+B
to build and F5 to debug. Every task runs from the repo root.

Caveats: the tasks hardcode the Visual Studio **Community** MSBuild path — edit
them for another edition. The fourth include path is required or IntelliSense
cannot find `GLFW/glfw3.h` used by `src/demo_controls.c`.

## Verification

TESTED here:

- Per-file syntax check (no linking, all of `src/*.c` passes):
  ```sh
  gcc -fsyntax-only -I include -I raylib/src -I raylib/src/external/glfw/include src/<file>.c
  ```
  The GLFW include path is needed by `src/demo_controls.c`. MinGW `gcc` is at
  `C:\MinGW\bin`.
- Full Debug build above, exit code 0, game launches and stays running.

UNTESTED / not usable on this machine:

- `tests/compile-test.sh` — needs bash **and** `pkg-config raylib` (absent here).
  It also globs `*.c` in the current directory while using `-I include`, so it
  only works from a directory containing both the sources and `include/`.
  Treat it as a prototype; the `gcc -fsyntax-only` command above is the
  reliable substitute.
- `make` / gmake builds (`docs/building/gmake.md`, `w64devkit.md`) — no `make`
  installed here.
- There is **no automated test suite** and no unit tests. Behaviour must be
  confirmed by playing the game.

## Manual gameplay checklist

Launch from the repo root, then verify and report only what you actually see:

1. Intro screen advances with Enter/Space.
2. Paddle follows the mouse by default; `M` toggles mouse/keyboard control;
   Left/Right (or A/D) move it in keyboard mode.
3. Space releases the ball from the paddle.
4. Ball bounces off the top, left, right walls and the paddle.
5. Hitting a block removes it and increases the score.
6. Clearing all blocks advances the level; losing a ball costs a life; zero
   lives leads to the scoreboard.
7. Scoreboard: type a name, Enter submits; Enter/Escape exits.
8. Debug toggles: `R` reverse paddle, `Z` shrink, `X` grow.
9. Collision sounds play.
10. Closing the window exits cleanly.

## Architecture

| Path | Responsibility |
| --- | --- |
| `src/rayboing_demo.c` | `main`, 575x720 window, game-mode loop, input dispatch, shutdown |
| `src/demo_gamemodes.c` | mode state machine (INITGAME/PLAY/WIN/LOSE/CANCEL/EXIT), lives, timer, scoring |
| `src/demo_ball.c` | ball state, movement, wall/paddle/block collision, speed boost |
| `src/demo_blockloader.c` | play-area geometry, level parsing, block textures/types, collision rects |
| `src/paddle.c`, `src/demo_controls.c` | paddle motion and input mapping |
| `src/audio.c`, `src/intro.c`, `src/scoreboard.c` | sound, intro screen, high scores |

Headers mirror `src/` in `include/`. Levels are ASCII grids in
`resource/levels/*.data` (9 columns x 15 rows).

Gameplay geometry (worth knowing for collision work): window 575x720,
`PLAY_X_PADDING` 40 and `PLAY_Y_PADDING` 70 give a 495x580 play area; 9 columns
and 15 rows plus `PADDLE_ROWS` 3 yield `colWidth` 55 and `rowHeight` 32; blocks
are 40x20 and the ball texture is 20x19, so gaps are 15 px horizontally and
12 px vertically.

## Windows quirks

- Always run the game from the repo root or assets, levels, and `scores.txt`
  resolve incorrectly.
- `premake5.exe` is a downloaded tool and is gitignored — never commit it.
- `build_files/` is generated and gitignored; `xboing.sln` is tracked.
- Git Bash / WSL `bash` exists, but there is no `make` or `pkg-config`.
- Display scaling: the window is a fixed 575x720 **physical** pixels. On the
  reference machine (2560x1600 at 125%) it looks small, and DPI-unaware capture
  tools report virtualized, cropped coordinates.
- Root-level `audio.c` and `version.c` are legacy leftovers; the build only
  compiles `src/**.c` and `include/**.h`.

## Git

```sh
git switch -c <issue-branch> main   # one issue per branch
git status                          # before editing
git diff                            # before committing
```
