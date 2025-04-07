workspace "Omaha AOF solver"
    architecture "x64"
    startproject "Solver"

    configurations
    {
        "Debug",
        "Release"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    IncludeDirs = {}
    IncludeDirs["PHeval"]="%{wks.location}/3rd/PHeval/include"

    LibDirs={}
    LibDirs["PHeval_d"]="%{wks.location}/3rd/PHeval/Debug"
    LibDirs["PHeval_r"]="%{wks.location}/3rd/PHeval/Release"

    include "Solver/solver_build.lua"
    --TODO: rename file