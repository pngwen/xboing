# Building on Windows
1. Select a project platform (e.g. Visual Studio, GMake).
2. Install build tools (e.g. MSBuild, Visual Studio)
3. Build target (e.g. all, raylib, rayboing)

### Build Guides
| **Project platform:** | [Visual Studio][Visual Studio Platform] | [GMake] |
| --- | --- | -- |
| **Build tools:** | Visual Studio, MSBuild | [MinGW-W64]

[Visual Studio Platform]:(#generate_project_for_visual_studio)
[GMake]:(#generate-project-for-mingw-w64)
[MinGW-W64]:(#building-project-with-mingw-w64)

## Getting source code
Clone the repository with the `--recursive` option.
```powershell
# Ensures submodules are also cloned
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
├── premake5.exe
└── premake5.lua
```

### Premake usage
Use **premake5** with the `--help` to list project arguments.
```powershell
.\premake5.exe --help
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

## Installing MinGW-W64 via W64Devkit

> [!NOTE]
> **[raylib/wiki/Working-on-Windows](https://github.com/raysan5/raylib/wiki/Working-on-Windows):** \
> The best way to get MinGW-W64 and GCC is via the W64Devkit \
> We do not recommend using MSYS2 to get GCC, even though several tutorials suggest it. It can be difficult to configure properly and does not always include all the things needed for a complete development system. The W64-Devkit is a much better way to get the needed toolchain for building games with raylib.

* Download W64Devkit archive from [here](https://github.com/skeeto/w64devkit/releases).
* Extract W64Devkit (e.g. *.7z.exe).
* Navigate to extracted files and run `w64devkit.exe`.

## Generate project for MinGW-W64
Requires [installing MinGW-W64 via W64Devkit](#installing-mingw-w64-via-w64devkit):

>`w64devkit.exe` will open a terminal that can be used to run our generated **make** commands.

To generate a project for MinGW-W64, use **premake gmake**.
```powershell
# premake5 [action]
.\premake5 gmake
```
Premake will generate a Makefile in the project directory with all configured project targets.

---

Open `w64devkit.exe` to use **make**. Use **make help** to list configuration options and build targets.
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

## Building project with MinGW-W64
Requires [installing MinGW-W64 via W64Devkit](#installing-mingw-w64-via-w64devkit):
>`w64devkit.exe` will open a terminal that can be used to run our generated **make** commands.

Open `w64devkit.exe` to use **make**. Targets can be built inside of the terminal with the command **make [target]**
```sh
# Default to all targets
make

# Rayboing project
make rayboing

# Raylib project (static lib)
make raylib
```

Configurations can be selected with **make [config=name]**
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
