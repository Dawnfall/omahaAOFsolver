project "Tests"
    kind "ConsoleApp"
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
        "%{IncludeDirs.PHeval}",
        --"%{IncludeDirs.VCPKG}"
    }

    links
    {
       "Solver"
    }

    filter "configurations:Debug"
        libdirs 
        {
            "%{LibDirs.PHeval_d}",
            --"%{LibDirs.VCPKG_D}"
        }
        links
        {
            "phevalplo4.lib",
            --"gtestd.lib"  
        }
        defines "SOLVER_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        libdirs 
        {
            "%{LibDirs.PHeval_r}",
            --"%{LibDirs.VCPKG_R}"
        }
        links
        {
            "phevalplo4.lib",
            --"gtest.lib",   
        }   
        defines "SOLVER_RELEASE"
        runtime "Release"
        optimize "on"
