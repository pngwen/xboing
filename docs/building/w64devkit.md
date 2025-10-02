# Building with Make (W64Devkit)
## Dependencies
Windows does not regularly include standard GNU tools, including GCC and Make, in their installations. To build with GCC and Make on Windows, the [W64Devkit](https://github.com/skeeto/w64devkit) provides a small collection of developer tools including `make`, `gcc` and `sh`. You can get W64Devkit [here](https://github.com/skeeto/w64devkit/releases/tag/v2.4.0).
 * Running the downloaded executable will extract W64Devkit to your selected folder. Navigate to the installation folder and run `w64devkit.exe` to launch the W64Devkit console.
 * All required tools should be available within this console.
* Navigate to the xboing project root to begin the build process.

## Generate project for W64Devkit Make
To generate a project for W64Devkit, use **premake gmake**. *This can be done from Powershell, or from the W64Devkit console*.
```sh
# premake5 [action]
.\premake5.exe gmake
```
Premake will generate a Makefile in the project directory with all project targets configured.

## Building project with W64Devkit Make
Targets can be built with **make [target]**. *This can only be from the W64Devkit console*.
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
