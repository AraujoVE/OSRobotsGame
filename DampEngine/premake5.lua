project "DampEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "depch.hpp"
	pchsource "src/depch.cpp"
	
	defines {
		DE_ENGINE_BUILD
	}

	files {
		"src/DampEngine/**.cpp",
		"src/DampEngine/**.hpp",
		"src/ImGui/**.cpp",
		"src/ImGui/**.hpp",

		--TODO: separated section for OpenGL
		"src/Platform/OpenGL/**.hpp",
		"src/Platform/OpenGL/**.cpp",
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui"
		-- "opengl32.lib" Windows?
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			DE_SYSTEM_WINDOWS
		}

	filter "system:linux"
		defines {
			DE_SYSTEM_LINUX
		}
		
		files {
			"src/Platform/Linux/**.cpp",
			"src/Platform/Linux/**.hpp",
		}

		links {
			"dl",
			"pthread"
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