# Building on Windows

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
* Download premake5 binaries for your platform [here](https://premake.github.io/download).

* Extract the downloaded archive and place the contents within the root of the xboing project.

The `premake5` executable should be in the same directory as the `premake5.lua` file included in the repository:

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

## Choose toolset
| **Toolchain** | **Guides** |
| --- | --- |
| Visual Studio | [(install)](#installing-visual-studio) [(generate)](#generate-project-for-visual-studio) [(build)](#building-project-with-visual-studio-ide) |
| MSBuild (standalone) | [(install)](#installing-msbuild) [(generate)](#generate-project-for-visual-studio) [(build)](#building-project-with-msbuild) |
| GMake (mingw-w64) | [(install)](#installing-mingw-w64-via-w64devkit) [(generate)](#generate-project-for-mingw-w64) [(build)](#building-project-with-mingw-w64) |

## Installing Visual Studio
https://visualstudio.microsoft.com/vs/community/

## Generate project for Visual Studio
To generate a project for Visual Studio or MSBuild, use **premake vs\*** (vs2022, vs2019)
```powershell
# Generate files for Visual Studio 2022
.\premake5 vs2022
```
Premake will generate a Visual Studio solution file in the project directory with all configured project targets.

## Building project with Visual Studio IDE
Requires [installing Visual Studio](#installing-visual-studio).
1. Open the project solution (`.sln`) in Visual Studio.
2. Select `Build > Build Solution` in the editor.

## Building project with MSBuild
1. Install [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/?q=build+tools#build-tools-for-visual-studio-2022) from the Visual Studio Installer.
    * `MSBuild` will be installed to  `%ProgramFiles%\Microsoft Visual Studio\(version)\BuildTools\MSBuild`.
    * `MSBuild.exe` can be found in `MSBuild\Current\Bin\`
3. Run **msbuild [solution]**.
```powershell
MSBuild.exe .\xboing.sln
```

## Installing MinGW-W64 via W64Devkit
> [!NOTE]
> #### [raylib/wiki/Working-on-Windows](https://github.com/raysan5/raylib/wiki/Working-on-Windows)
> The best way to get MinGW-W64 and GCC is via the W64Devkit.
> We do not recommend using MSYS2 to get GCC, even though several tutorials suggest it. It can be difficult to configure properly and does not always include all the things needed for a complete development system. The W64-Devkit is a much better way to get the needed toolchain for building games with raylib.

* Download W64Devkit archive from [here](https://github.com/skeeto/w64devkit/releases).
* Extract W64Devkit (e.g. *.7z.exe).
* Navigate to extracted files and run `w64devkit.exe`.

## Generate project for MinGW-W64
Requires [installing MinGW-W64 via W64Devkit](#installing-mingw-w64-via-w64devkit).

To generate a project for MinGW-W64, use **premake gmake**.
```powershell
# In powershell

# Generate files for GMake
.\premake5 gmake
```
Premake will generate a Makefile in the project directory with all configured project targets.

### MinGW-W64 usage

Open `w64devkit.exe` to use **make**. Use **make help** to list configuration options and build targets.
```sh
# In w64devkit.exe terminal

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
Requires [installing MinGW-W64 via W64Devkit](#installing-mingw-w64-via-w64devkit).

Open `w64devkit.exe` to use **make**. Targets can be built inside of the terminal with the command **make [target]**.
```sh
# In w64devkit.exe terminal

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

Completed compilation units are saved to `bin/`.
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

> [!WARNING]
> Unfinished

* Build and run the **rayboing** target using make/mingw-w64.
---
* Clean make outputs.
```sh
# In w64devkit.exe terminal

make clean
```
