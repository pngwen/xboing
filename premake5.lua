workspace "xboing"
    configurations { "Debug", "Release" }
    includedirs { "include" }

    filter "configurations:Debug"
        symbols "On"
    
    filter "configurations:Release"
        optimize "On"
    
    filter "system:macosx"
        buildoptions { "`pkg-config raylib --cflags`" }
        linkoptions { "`pkg-config raylib --libs`" }
    
    filter {}
    
    project "Rayboing"
        kind "WindowedApp"
        language "C"
        targetdir "bin/%{cfg.buildcfg}"
        
        files { "src/rayboing/*.c" }
        files { "src/paddle.c" }
        removefiles { "src/rayboing/blockloader.c", "src/rayboing/paddle_demo.c" }
        
        includedirs { os.findheader("raylib") }
        libdirs { os.findlib("raylib") }
        links { "raylib" }
