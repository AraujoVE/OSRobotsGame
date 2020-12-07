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
		"%{wks.location}/DampEngine/src",
		"%{wks.location}/DampEngine/vendor/spdlog/include"
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