project "DampEngine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	platforms { 
		"OpenGL"
	}

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "depch.hpp"
	pchsource "src/depch.cpp"

	files {
		"src/DampEngine/**.cpp",
		"src/DampEngine/**.hpp"		
	}
	
	defines {}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links {
		"GLFW"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			OS_SYSTEM_WINDOWS
		}

	filter "system:linux"
		defines {
			OS_SYSTEM_LINUX
		}
		
		files {
			"src/Platform/Linux/**.cpp",
			"src/Platform/Linux/**.hpp"
		}

	filter "platforms:OpenGL"
		
		files {
			"src/Platform/OpenGL/**.cpp",
			"src/Platform/OpenGL/**.hpp"
		}
	

	filter "configurations:Debug"
		defines "DE_ENGINE_BUILD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DE_ENGINE_BUILD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DE_ENGINE_BUILD_DIST"
		runtime "Release"
		optimize "on"