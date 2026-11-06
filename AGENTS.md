# AGENTS.md

## Build & run (macOS)
- **Clone**: `git clone https://github.com/zahcwa/preboing.git --recurse-submodules`
- **Premake**: execute the `premake5` binary that lives in the repo root; if missing download the latest and put in `$PATH`.
- **Generate**: `./premake5 gmake`
- **Build**: `make -f Makefile`
- **Run**: `./bin/Release/rayboing`

## Common pitfalls
- **Submodule missing** – run `git submodule update --init` when `raylib` files are absent.
- **Premake binary not executable** – `chmod +x premake5`.
- **macOS system frameworks** – `xcode-select --install` must be run first.
- **Premake version mismatch** – use the diesel 5.0.1‑alpha3; older versions may create deprecated Makefiles.
- **Wrong config for debug/release** – `make` uses `Debug` by default; to get Release use `make config=Release`.
- **Missing C++ standard** – the project is C only; no need to set `-std=c++`.
- **OpenGL 3.3** – macOS 13+ requires `-framework OpenGL`; `raylib.lua` already does this.

## Minimal agent guidance
- Do not edit the `raylib` submodule or generated `Makefile`.
- Use the branch name `macos-build-fix` for any PR.
- Commit messages should reference the issue if one exists.
- Add `Co‑authored‑by: OpenCode (…)` to each commit line.
