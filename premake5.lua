-- premake5.lua

workspace "WinApp"
	architecture "x64"

	configurations{
		"Debug",
		"Release"
	}

	startproject "Wind"

	flags{
		"MultiProcessorCompile",
		"FloatFast"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Wind"
	
	location "Wind"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	
	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"