workspace "OSRobotGame"
    architecture "x64"
    startproject "Application"
    
    configurations { 
        "Debug", 
        "Release", 
        "Dist",
    }

    flags {
		"MultiProcessorCompile"
	}

	IncludeDir = {}
	IncludeDir["GLFW"] = "%{wks.location}/DampEngine/vendor/GLFW/include"
	IncludeDir["Glad"] = "%{wks.location}/DampEngine/vendor/Glad/include"

    -- This defines ... in
    --./bin/ ... /{projectName}
    -- used by projects
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Dependencies"
    	-- include "vendor/premake" CUSTOMIZATIONS (currently none)
    	include "DampEngine/vendor/GLFW"
    	include "DampEngine/vendor/Glad"
    group ""    

    include "DampEngine"
    include "Application"
    