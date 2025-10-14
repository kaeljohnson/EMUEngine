workspace "gameV1"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputDirectory = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Emu"
    location "Emu"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "{%prj.name}/public/**.h",
        "%{prj.name}/include/**.h",
        "%{prj.name}/source/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/external/vcpkg/installed/x64-windows/include",
        "%{prj.name}/external/spdlog/include",
        "%{prj.name}/external/box2d/build/install/include"
    }

    libdirs
	{
		"%{prj.name}/external/vcpkg/installed/x64-windows/lib",
		"%{prj.name}/external/box2d/build/install/lib"
	}

    links
	{
		"SDL2",
		"/manual-link/SDL2main",
		"SDL2_image",
        "SDL2_mixer",
		"box2dd"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions { "/utf-8" }

        defines
        {
            "EMU_BUILD_DLL"
        }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG"}

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG"}

    filter "configurations:Dist"
        optimize "On"
        defines { "NDEBUG" }

project "GameV1"
    location "GameV1"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "Emu/public",
        "Emu/external/vcpkg/installed/x64-windows/include",
    }

    libdirs
	{
		"Emu/external/vcpkg/installed/x64-windows/lib",
	}

    links
    {
        "Emu",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG"}

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG"}

    filter "configurations:Dist"
        optimize "On"
        defines { "NDEBUG" }


project "EmuTests"
    location "EmuTests"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "Emu/public",
        "Emu/external/vcpkg/installed/x64-windows/include",
    }

    libdirs
    {
        "Emu/external/vcpkg/installed/x64-windows/lib",
    }

    links
    {
        "Emu",
        "gtest",
        "gtest_main"
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter "configurations:Dist"
        optimize "On"
        defines { "NDEBUG" }
