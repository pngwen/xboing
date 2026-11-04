# Xboing / Rayboing

C raylib port of XBoing. Active target is `rayboing` (C console app). Premake5 generates GNU Make / VS project files. The `archive/` directory is the legacy X11 implementation and is **not** part of the build (reference only).

## Build (Linux)
Prerequisites: C compiler + X11/Wayland, GL/EGL, ALSA dev libs (see `.devcontainer/devcontainer.json` apt packages). Also needs the `raylib` submodule:

```sh
git submodule update --init
./premake5 gmake          # download premake5 to repo root if missing
make rayboing             # or: make            (all), make raylib
make rayboing config=release
```

Outputs go to `bin/Debug/rayboing` and `bin/Release/rayboing`. Run from the repo root — resource paths are relative (`resource/...`).

Premake output (`Makefile`, `*.make`, `obj/`) is gitignored; regenerate after changing `premake5.lua` or `build_files/raylib.lua`.

## Run
```sh
./bin/Debug/rayboing                      # intro screen, then resource/levels/level01.data
./bin/Debug/rayboing resource/levels/level03.data
```
Window is fixed `575x720` (see `SCREEN_WIDTH`/`SCREEN_HEIGHT` in `src/rayboing_demo.c`).

## Source layout
- `src/*.c`, `include/*.h` — active game code (`rayboing_demo.c` is the entry point).
- `src/demo_*.c` — feature modules (ball, block loader, game modes, controls).
- `archive/src`, `archive/include` — original X11 gold reference; not compiled. Recent feature PR notes refer to files there.
- `resource/` — textures (PNG), sounds (MP3, case-sensitive names), level data (`resource/levels/levelNN.data`).
- `tests/compile-test.sh` — compiles top-level `*.c` only (legacy; does not build `src/`).

## Gotchas
- Run generated binary from repo root; `LoadTexture`/`LoadSound` paths are relative.
- Asset paths are case-sensitive on Linux; `initAudioFiles()` returns `false` on any missing sound file, which aborts the startup chain in `rayboing_demo.c`. Keep code string casing matched to disk (e.g. `Doh1.mp3`, not `doh1.mp3`).
- `demo_controls.h` pulls in `<GLFW/glfw3.h>` via the raylib bundle; compile `src/` only through the generated Makefile (plain `gcc` lacks those include paths).
- `raylib` is pinned to a specific commit; if the submodule is missing, build fails immediately.
- High scores are read/written as `scores.txt` in the current working directory; there is no installed data path.

## Contributing
Workflow docs: `docs/project/workflow.md`, `docs/project/policies.md`. One issue per branch; sync `main` before opening a PR and include the issue number. Build with the GNU Make guide in `docs/building/gmake.md` before submitting.