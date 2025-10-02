# Building with MSBuild
## Dependencies
Windows users who have previously installed most recent versions of Visual Studio already have MSBuild installed. MSBuild can usually be found under the Visual Studio installation folder within *MSBuild/Current/Bin*.
##### Example location:
```
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe
```
MSBuild can be added to the system path for easier access. For users who have not installed Visual Studio, MSBuild can be installed [here](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022).

## Generate project for Visual Studio
To generate a project for Visual Studio, use **premake vs2022**.
```sh
# premake5 [action]
.\premake5.exe vs2022
```
Premake will generate a solution file in the project directory with all project targets configured.

## Building project with MSBuild
Targets can be built with **MSBuild [solution.sln]**.
```sh
# Default to all targets
MSBuild.exe xboing.sln

# Rayboing project
MSBuild.exe xboing.sln -t:rayboing

# Raylib project (static lib)
MSBuild.exe xboing.sln -t:raylib
```

Configurations can be selected with **make [config=name]**.
```sh
MSBuild.exe xboing.sln -t:rayboing -p:Configuration=Release
```

Completed compilation units are saved to `bin/`
```
bin/
├── Debug
│   ├── raylib.lib
│   └── rayboing.exe
└── Release
    ├── raylib.lib
    └── rayboing.exe
```
