project "Solver"
    kind "StaticLib"
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
        "%{IncludeDirs.PHeval}"
    }

    links
    {
        "phevalplo4.lib"
    }

filter "configurations:Debug"
    libdirs 
    {
        "%{LibDirs.PHeval_d}"
    }
    defines 
    {
        "DEBUG",
        "SOLVER_DEBUG"
    }
    runtime "Debug"
    symbols "On"
    optimize "Off"

filter "configurations:Release"
    libdirs 
    {
        "%{LibDirs.PHeval_r}"
    }
    defines 
    { 
        "NDEBUG",
        "SOLVER_RELEASE"
    }
    runtime "Release"
    symbols "On" -- Enable debugging symbols for runtime inspection
    optimize "Speed" -- Optimize for speed but allow some inspection

filter "configurations:Deploy"
    libdirs 
    {
        "%{LibDirs.PHeval_r}"
    }
    defines
    {
        "NDEBUG",
        "SOLVER_RELEASE"
    } 
    runtime "Release"
    symbols "Off"        -- Disable debugging symbols
    optimize "Full"      -- Maximum optimization for deployment
