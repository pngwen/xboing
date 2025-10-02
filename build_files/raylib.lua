local raylib = {}

raylib.submodule = "raylib"
raylib.platform = {
    "PLATFORM_DESKTOP",
    "GRAPHICS_API_OPENGL_33",
}

-- Link with raylib.
function raylib.links()
    links { "raylib" }

    -- Platform dependencies
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

-- Include raylib files.
function raylib.include()
    includedirs {
        raylib.submodule .. "/src" ,
        raylib.submodule .. "/src/external",
        raylib.submodule .. "/src/external/glfw/include"
    }
    flags { "ShadowedVariables" }
end

function raylib.link_homebrew() -- Link with homebrew installation. TODO: make into option
    print("Configuring for homebrew installation...")

    links {}
    buildoptions { "`pkg-config raylib --cflags`" }
    linkoptions { "`pkg-config raylib --libs`" }
end

-- Configure project for raylib use.
--
-- Mostly adapted from raylib-extras/raylib-quickstart.
function raylib.setup_project()
    raylib.links()
    raylib.include()

    defines (raylib.platform)

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
    filter {}
end

-- Create raylib static lib target/project.
function raylib.static_lib_target()
        kind "StaticLib"
        language "C"

        location "build_files"
        targetdir "bin/%{cfg.buildcfg}"

        defines (raylib.platform)

        includedirs {
            raylib.submodule .. "/src",
            raylib.submodule .. "/src/external/glfw/include"
        }

        files { raylib.submodule .. "/src/*.c", raylib.submodule .. "/src/*.h" }

        removefiles {raylib.submodule .. "/src/rcore_*.c"}

        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
            characterset ("Unicode")
            buildoptions { "/Zc:__cplusplus" }
        filter { "system:macosx", "files:" .. raylib.submodule .. "/src/rglfw.c" }
            compileas "Objective-C"
        filter {"system:linux"}
            defines {"_GLFW_X11"}
        filter {}
end

return raylib
