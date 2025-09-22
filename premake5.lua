workspace "Omaha AOF solver"
    architecture "x64"
    startproject "App"

    configurations
    {
        "Debug",
        "Release",
        "Deploy"
    }

    outputDir = "%{cfg.buildcfg}"
    
    IncludeDirs = {}
    IncludeDirs["Solver"]="%{wks.location}/Solver/src"
    IncludeDirs["App"]="%{wks.location}/App/src"
    IncludeDirs["PHeval"]="%{wks.location}/3rd/PHeval/include"
    IncludeDirs["VCPKG"]="%{wks.location}/vcpkg_installed/x64-windows/include"
    
    LibDirs={}
    LibDirs["PHeval_d"]="%{wks.location}/3rd/PHeval/libs/Debug"
    LibDirs["PHeval_r"]="%{wks.location}/3rd/PHeval/libs/Release"
    LibDirs["VCPKG_D"]="%{wks.location}/vcpkg_installed/x64-windows/debug/lib"
    LibDirs["VCPKG_R"]="%{wks.location}/vcpkg_installed/x64-windows/lib"

    include "Solver/solver_build.lua"
    include "App/app_build.lua"
    include "Tests/tests_build.lua"
    --TODO: rename file