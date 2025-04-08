workspace "Omaha AOF solver"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "App"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    IncludeDirs = {}
    IncludeDirs["Solver"]="%{wks.location}/Solver/src"
    IncludeDirs["App"]="%{wks.location}/App/src"
    IncludeDirs["PHeval"]="%{wks.location}/3rd/PHeval/include"

    LibDirs={}
    LibDirs["PHeval_d"]="%{wks.location}/3rd/PHeval/Debug"
    LibDirs["PHeval_r"]="%{wks.location}/3rd/PHeval/Release"

    include "Solver/solver_build.lua"
    include "App/app_build.lua"
    --TODO: rename file