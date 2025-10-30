local raylib = require "build_files.raylib"

workspace "xboing"
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter {}

    startproject "rayboing"

    project "rayboing"
        kind "ConsoleApp"
        language "C"
        location "build_files"
        targetdir "bin/%{cfg.buildcfg}"

        raylib.setup_project()

        includedirs { "include", "src" }

        vpaths {
            ["Header Files/*"] = { "include/**.h" },
            ["Source Files/*"] = { "src/**.c" }
        }

        files { "src/**.c", "include/**.h"}

    project "raylib"
        raylib.static_lib_target()
