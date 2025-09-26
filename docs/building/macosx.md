# Building on MacOS
This guide assumes you are using clang on macos.

## Getting source code
Clone the repository using the `--recursive` option to also clone all submodules.
```sh
git clone https://github.com/zahcwa/preboing.git --recursive
```
Your project directory should look something like this:
```
xboing
├── include
├── raylib
│   └── src
└── src
```

## Installing premake5
* Download premake5 binaries for your platform [here.](https://premake.github.io/download)

* Extract the downloaded archive place the contents within the root of the xboing project.

The `premake5` executable should be in the same directory as the `premake5.lua` file:

```
xboing
├── include
├── raylib
│   └── src
├── src
├── premake5
└── premake5.lua
```

## Premake usage
Use **premake5** with the `--help` to list project arguments.
```sh
./premake5 --help
```
```
Premake 5.0.0, a build script generator
...

Usage: premake5 [options] action [arguments]
...

ACTIONS
  clean
  gmake
  vs2022
  ...
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

### Building project with GNU Make
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

## Examples

* Build and run the **rayboing** target using make.

```sh
# Generate make configuration.
./premake5 gmake

# Build rayboing target with make.
make rayboing

# Run rayboing [level]
./bin/Debug/rayboing src/rayboing/levels/level01.data
```
---
* Clean make outputs.
```sh
make clean
```
