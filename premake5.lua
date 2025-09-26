local raylib_module = "raylib"

local platform = {
    "GRAPHICS_API_OPENGL_33", -- Graphics API: opengl33
    "PLATFORM_DESKTOP" -- Backend: GLFW
}

function links_raylib()
    links { "raylib" }

    filter "system:macosx"
        links {
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreFoundation.framework",
            "CoreVideo.framework",
            "AudioToolbox.framework"
        }
    filter "action:vs*"
        defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
        dependson {"raylib"}
        links {"raylib.lib"}
        characterset ("Unicode")
        buildoptions { "/Zc:__cplusplus" }
    filter "system:windows"
        defines{"_WIN32"}
        links {"winmm", "gdi32", "opengl32"}
        libdirs {"bin/%{cfg.buildcfg}"}
    filter "system:linux"
        links {"pthread", "m", "dl", "rt", "X11"}
    filter {}
end

function includes_raylib()
    includedirs {
        raylib_module .. "/src" ,
        raylib_module .. "/src/external",
        raylib_module .. "/src/external/glfw/include"
    }
    flags { "ShadowedVariables" }
end

function link_raylib_homebrew() -- Deprecated
    print("Configuring for homebrew installation...")

    links {}
    buildoptions { "`pkg-config raylib --cflags`" }
    linkoptions { "`pkg-config raylib --libs`" }
end

function project_use_raylib()
    -- Mostly adapted from raylib-extras/raylib-quickstart.

    links_raylib()
    includes_raylib()

    filter { "system:windows", "configurations:Release", "action:gmake*" }
        kind "WindowedApp"
        buildoptions { "-Wl,--subsystem,windows" }
    filter { "system:windows", "configurations:Release", "action:vs*" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
    filter "action:vs*"
        debugdir "$(SolutionDir)"
    filter {"system:macosx"}
        disablewarnings {"deprecated-declarations"}
    filter {"system:linux"}
        defines {"_GLFW_X11"}
        defines {"_GNU_SOURCE"}
    filter {}
end

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

        project_use_raylib()
        defines (platform)

        includedirs { "include", "src" }

        vpaths {
            ["Header Files/*"] = { "include/**.h" },
            ["Source Files/*"] = { "src/**.c" }
        }

        files { "src/rayboing/*.c", "src/paddle.c", "include/**.h" }
        removefiles { "src/rayboing/blockloader.c", "src/rayboing/paddle_demo.c", "src/main.c" }

    project "raylib"
        kind "StaticLib"
        language "C"

        location "build_files"
        targetdir "bin/%{cfg.buildcfg}"

        defines (platform)

        includedirs {
            raylib_module .. "/src",
            raylib_module .. "/src/external/glfw/include"
        }

        files { raylib_module .. "/src/*.c", raylib_module .. "/src/*.h" }

        removefiles {raylib_module .. "/src/rcore_*.c"}

        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
            characterset ("Unicode")
            buildoptions { "/Zc:__cplusplus" }
        filter { "system:macosx", "files:" .. raylib_module .. "/src/rglfw.c" }
            compileas "Objective-C"
        filter {}
