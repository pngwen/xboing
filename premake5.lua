function useraylib()
    includedirs { os.findheader("raylib") }
    libdirs { os.findlib("raylib") }
    links { "raylib" }

    filter "system:macosx" -- Configured for use with homebrew.
        links {}
        buildoptions { "`pkg-config raylib --cflags`" }
        linkoptions { "`pkg-config raylib --libs`" }
    filter {}
end

workspace "xboing"
    configurations { "Debug", "Release" }
    includedirs { "include" }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter {}

    project "Rayboing"
        kind "WindowedApp"
        language "C"
        targetdir "bin/%{cfg.buildcfg}"

        files { "src/rayboing/*.c", "src/paddle.c", "include/**.h" }
        removefiles { "src/rayboing/blockloader.c", "src/rayboing/paddle_demo.c", "src/main.c" }

        useraylib()
