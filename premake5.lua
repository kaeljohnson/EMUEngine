workspace "v1"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.architecture}"

project "v1"
	location "v1"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDirectory .. "/%prj.name}")

	files
	{
		"%{prj.name}/Application/include/**.h",
		"%{prj.name}/Application/source/**.cpp"
		"%{prj.name}/Engine/include/**.h",
		"%{prj.name}/Engine/source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/external/sdl2/SDL2-2.24.1/include",
		"%{prj.name}/external/sdl2/SDL2_image-2.6.2/include"
		--- C:\Users\kaelj\OneDrive\Desktop\VisualStudioDevStuff\SDL2-2.24.1\include
		--- C:\Users\kaelj\OneDrive\Desktop\VisualStudioDevStuff\SDL2_image-2.6.2\include
	}

	libdirs
	{
		"%{prj.name}/external/sdl2/SDL2-2.24.1/lib/x64",
		"%{prj.name}/external/sdl2/SDL2_image-2.6.2/lib/x64"
		--- C:\Users\kaelj\OneDrive\Desktop\VisualStudioDevStuff\SDL2-2.24.1\lib\x64
		--- C:\Users\kaelj\OneDrive\Desktop\VisualStudioDevStuff\SDL2_image-2.6.2\lib\x64
	}


	links
	{
		"SDL2",
		"SDL2main",
		"SDL2_image"
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