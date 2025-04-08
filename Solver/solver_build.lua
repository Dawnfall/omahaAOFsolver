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
        --todo
    }

    filter "configurations:Debug"
    libdirs 
    {
        "%{LibDirs.PHeval_d}"
    }
    links
    {
        "phevalplo4.lib"
    }
    defines "SOLVER_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    libdirs 
    {
        "%{LibDirs.PHeval_r}"
    }
    links
    {
        "phevalplo4.lib"
    }    
    defines "SOLVER_RELEASE"
    runtime "Release"
    symbols "Off"        -- Disable debugging symbols
    optimize "Full"      -- Maximum optimization for deployment
