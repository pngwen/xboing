# AGENTS.md

## What this is
- Ongoing raylib/C port of XBoing. The buildable app is `src/rayboing_demo.c` plus the modules in `src/` and `include/`.
- `archive/` contains the original X11 implementation and spike/PR notes; it is not compiled.
- `raylib/` is a git submodule (raylib 5.5). Clone with `--recurse-submodules` or run `git submodule update --init`.
- The top-level `audio.c` symlink points to a missing `audio/SUNaudio.c` and is not part of the build; use `src/audio.c`.

## Directory map
- `src/`, `include/`: current port. Module names are prefixed (`demo_ball`, `demo_blockloader`, etc.).
- `resource/`: runtime assets, including levels (`resource/levels/level*.data`), textures, and sounds.
- `build_files/raylib.lua`: tracked premake script used to generate the build.
- `tests/`: ad hoc compile check only; no CI.
- `docs/`: setup, build, workflow, and project-policy docs.

## Build
- Generate: `premake5 gmake`
- Build: `make` (`make rayboing`, `make raylib`, `make rayboing config=release`)
- Output: `bin/Debug/rayboing` or `bin/Release/rayboing`
- Generated files are gitignored: `Makefile`, `bin/`, `obj/`, `build_files/*.make`

## Running
- Run from the repository root. Asset paths such as `resource/levels/level01.data` are resolved relative to the current working directory.
- Default level: `resource/levels/level01.data`; any other level can be passed as the single CLI argument.

## Tests and verification
- There is no test suite or CI. `tests/compile-test.sh` compiles each `src/*.c` independently and is the only automated-ish check.
- Its include path does not match this tree; check files with:
  `cd src && gcc -I ../include -I ../raylib/src -I ../raylib/src/external/glfw/include -c <file>.c`
- The listed compile blockers have been resolved; update this section if new ones are found.

## Coding conventions actually present
- No formatter, linter, or editor config exists; do not assume a house style. Match the surrounding file instead of reformatting it.
- Headers use include guards (`*_H_` or `_NAME_H_`), with declarations in `include/` and implementations in `src/`.
- Modules are lowercase/underscore names; public APIs are PascalCase/CamelCase and helper prototypes are often declared near the top of the `.c` file.
- Mixed tabs and spaces occur; preserve existing indentation in the file being edited.

## Project management and workflow
- Branch and PR expectations: `docs/project/workflow.md`.
- Team constraints: `docs/project/policies.md` (one in-progress item per coder; claim issues by owner; include issue number in PR).
- Platform-specific build instructions: `docs/building/*.md`.

## Notes
- `docs/setup/getting-started.md` describes the premake-and-submodule workflow.
- Changes in `archive/` do not affect the raylib build; keep them separate from the port.
