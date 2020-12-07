project "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.cpp",
		"src/**.hpp"
	}

	defines {}

	includedirs {
		"%{wks.location}/DampEngine/src"
	}

	libdirs {
		"%{wks.location}/bin/" .. outputdir .. "/DampEngine"
	}

	links {
		"DampEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			OS_SYSTEM_WINDOWS
		}

	-- filter "system:"

	filter "configurations:Debug"
		defines "OS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OS_DIST"
		runtime "Release"
		optimize "on"