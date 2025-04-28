project "App"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20" --c++latest ??TODO??
    staticruntime "off"
    systemversion "latest"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        
    }
    
    includedirs
    {
        "%{IncludeDirs.Solver}",
        "%{IncludeDirs.App}",
        "%{IncludeDirs.PHeval}",
        "%{IncludeDirs.VCPKG}"
    }

    links
    {
       "Solver",
        "user32", 
        "gdi32", 
        "shell32"
    }

    filter "configurations:Debug"
        libdirs 
        {
            "%{LibDirs.PHeval_d}",
            "%{LibDirs.VCPKG_D}",
        }
        links
        {
            "phevalplo4.lib",
            "fltkd.lib",
            "fltk_imagesd.lib",
            "zlibd.lib",
            "libpng16d.lib"
        }
        defines "SOLVER_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        libdirs 
        {
            "%{LibDirs.PHeval_r}",
            "%{LibDirs.VCPKG_R}"
        }
        links
        {
            "phevalplo4.lib",
            "fltk.lib",
            "fltk_images.lib",
            "zlib.lib",
            "libpng16.lib"
        }   
        defines "SOLVER_RELEASE"
        runtime "Release"
        optimize "on"
