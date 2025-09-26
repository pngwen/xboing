local raylib_module = "raylib"

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
    filter {}
end

function includes_raylib()
    includedirs { raylib_module .. "/src" }
end

function link_raylib_homebrew()
    print("Configuring for homebrew installation...")

    links {}
    buildoptions { "`pkg-config raylib --cflags`" }
    linkoptions { "`pkg-config raylib --libs`" }
end

newoption {
    trigger = "force-homebrew",
    description = "Use homebrew-installed raylib",
}

workspace "xboing"
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter {}

    project "raylib"
        kind "StaticLib"
        language "C"
        cdialect "C99"

        location (raylib_module)
        targetdir ("bin/%{cfg.buildcfg}")

        -- Graphics: opengl33, Backend: GLFW
        defines { "GRAPHICS_API_OPENGL_33", "PLATFORM_DESKTOP" }

        includedirs {
            raylib_module .. "/src",
            --raylib_module .. "/src/external",
            raylib_module .. "/src/external/glfw/include"
        }

        files { raylib_module .. "/src/*.c", raylib_module .. "src/*.h" }

        removefiles {raylib_module .. "/src/rcore_*.c"}

        filter { "system:macosx", "files:" .. raylib_module .. "/src/rglfw.c" }
            compileas "Objective-C"
        filter {}

    project "rayboing"
        kind "WindowedApp"
        language "C"
        targetdir "bin/%{cfg.buildcfg}"

        includedirs { "include" }
        includes_raylib()

        files { "src/rayboing/*.c", "src/paddle.c", "include/**.h" }
        removefiles { "src/rayboing/blockloader.c", "src/rayboing/paddle_demo.c", "src/main.c" }

        filter { "system:macosx", "options:force-homebrew" }
            link_raylib_homebrew()
        filter { "system:macosx", "options:not force-homebrew" }
            links_raylib()
        filter {}

        -- useraylib()
