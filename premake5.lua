workspace "Pearly"
	architecture "x64"
	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Pearly/vendor/GLFW/include"

include "Pearly/vendor/GLFW"

project "Pearly"
	location "Pearly"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	pchheader "prpch.h"
	pchsource "Pearly/src/prpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"Pearly/src",
		"%{IncludeDirs.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines "PR_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines 
		{
			"PR_DEBUG",
			"PR_ENABLE_ASSERTS"
		}
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PR_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pearly/vendor/spdlog/include",
		"Pearly/src"
	}

	links
	{
		"Pearly"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines "PR_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "PR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PR_DIST"
		optimize "On"