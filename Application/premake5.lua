project "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mypch.hpp"
	pchsource "src/mypch.cpp"

	files {
		"src/**.cpp",
		"src/**.hpp"
	}

	defines {}

	includedirs {
		"src/",
		"%{wks.location}/DampEngine/src",
		"%{wks.location}/DampEngine/vendor/spdlog/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	
	links {
		"DampEngine",
		"GLFW",
		"Glad",
		"ImGui",
		"armadillo",
		-- "lmpack"
	}

	filter "system:linux"
		defines {
			DE_SYSTEM_LINUX
		}
		
		links {
			"dl",
			"pthread"
		}
		
		buildoptions {
			"-Wl,--stack,524288000"
        }


	filter "system:windows"
		systemversion "latest"

		defines {
			OS_SYSTEM_WINDOWS
		}

	-- filter "system:"

	filter "configurations:Debug"
		defines "DE_APP_BUILD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DE_APP_BUILD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DE_APP_BUILD_DIST"
		runtime "Release"
		optimize "on"