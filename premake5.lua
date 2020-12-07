workspace "OSRobotGame"
    architecture "x64"
    startproject "Application"
    
    configurations { 
        "Debug", 
        "Release", 
        "Dist" 
    }

    flags {
		"MultiProcessorCompile"
	}

    -- This defines ... in
    --./bin/ ... /{projectName}
    -- used by projects
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "DampEngine"
    include "Application"