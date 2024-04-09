workspace "gameV1"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.architecture}"

project "v1"
	location "gameV1"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Game/include/**.h",
		"%{prj.name}/Game/source/**.cpp",
		"%{prj.name}/Engine/include/**.h",
		"%{prj.name}/Engine/source/**.cpp",
		"%{prj.name}/Engine/include/Events/**.h",
		"%{prj.name}/Engine/source/Events/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/external/vcpkg/installed/x64-windows/include",
		"%{prj.name}/external/spdlog/include",
		"%{prj.name}/external/box2d/include"
	}

	libdirs
	{
		"%{prj.name}/external/vcpkg/installed/x64-windows/lib",
		"%{prj.name}/external/box2d/build/bin/Debug"
	}

	links
	{
		"SDL2",
		"/manual-link/SDL2main",
		"SDL2_image",
		"box2d"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"