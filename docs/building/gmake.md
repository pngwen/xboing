# Building with Make
## Dependencies
### Linux
Building requires Make and a C compiler. For systems with **apt**, you can install the **build-essential** package.
```sh
sudo apt install build-essential
```
Raylib also requires a few libraries to function: ALSA for audio, Mesa for accelerated graphics, and X11/Wayland for windowing.
```sh
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```
### Macos
Building requires Make and a C compiler. For Macos systems, you can install Make and Clang through the xcode toolchain.
```sh
xcode-select --install
```
## Generate project for GNU Make
To generate a project for GNU Make, use **premake gmake**.
```sh
# premake5 [action]
./premake5 gmake
```
Premake will generate a Makefile in the project directory with all project targets configured.

Use **make help** to list configuration options and build targets.
```sh
make help
```
```
Usage: make [config=name] [target]

CONFIGURATIONS:
  debug
  ...

TARGETS:
  all (default)
  clean
  rayboing
  raylib
...
```

## Building project with GNU Make
Targets can be built with **make [target]**.
```sh
# Default to all targets
make

# Rayboing project
make rayboing

# Raylib project (static lib)
make raylib
```

Configurations can be selected with **make [config=name]**.
```sh
make rayboing config=release
```

Completed compilation units are saved to `bin/`
```
bin/
├── Debug
│   ├── libraylib.a
│   └── rayboing
└── Release
    ├── libraylib.a
    └── rayboing
```
