# Getting started

## Getting source code
Clone the repository using the `--recurse-submodules` option to also checkout all submodules.
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
If the contents of the raylib folder are missing, try initializing the submodule:
```sh
git submodule init
git submodule update
```
## Installing Premake
This project uses Premake 5 to generate build files and project configurations. To install Premake:
  1. Download Premake [here](https://premake.github.io/download).
  2. Extract the executable to the project root.

The `premake5` executable should be in the same directory as the `premake5.lua` file included in the repository:
```
xboing
├── include
├── raylib
│   └── src
├── src
├── premake5 (or premake5.exe)
└── premake5.lua
```
## Generate project files
This project currently supports generating files for the GNU and Visual Studio toolchains. Try these guides for configuring and using generated files for your system:
| Platform | Guide |
| --- | --- |
| Windows | [Visual Studio], [MSBuild (Standalone)][MSBuild], [Make (W64Devkit)][W64Devkit]
| Linux, Macos | [Make] |
> **Visual Studio Code** users on Windows should prefer using MSBuild or W64Devkit.

[Visual Studio]: ../building/visual-studio.md
[MSBuild]: ../building/msbuild.md
[W64Devkit]: ../building/w64devkit.md
[Make]: ../building/gmake.md
