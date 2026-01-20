workspace "EMUEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }

outputDirectory = "%{cfg.buildcfg}-%{cfg.architecture}"

-- =========================
-- Command-line options
-- =========================
newoption {
    trigger = "engine-only",
    description = "Generate only the engine project"
}

newoption {
    trigger = "tests",
    description = "Generate the tests with the engine"
}

-- =========================
-- Core workspace + engine
-- =========================
-- Emu Engine
project "Emu"
    location "Emu"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

    files
    {
        "%{prj.name}/public/**.h",
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
        "SDL2_image",
        "SDL2_mixer",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        defines { "DEBUG" }
        links { "box2dd" }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        defines { "NDEBUG" }
        links { "box2d" }

    filter "configurations:Distribution"
        runtime "Release"
        optimize "On"
        defines { "NDEBUG" }
        links { "box2d" }

-- =========================
-- Optional projects
-- =========================
if not _OPTIONS["engine-only"] then
    -- Sandbox Executable
    project "SandBox"
        location "SandBox"
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
        }

        links
        {
            "Emu",
        }

        filter "system:windows"
            cppdialect "C++20"
            systemversion "latest"
            postbuildcommands
            {
                '{COPYDIR} "../Emu/external/vcpkg/installed/x64-windows/bin" "%{cfg.targetdir}"'
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
            defines { "DEBUG" }

        filter "configurations:Release"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }

        filter "configurations:Distribution"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }

    -- EmuTests Executable
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
            "Emu/public"
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
            runtime "Debug"
            symbols "On"
            defines { "DEBUG" }

        filter "configurations:Release"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }

        filter "configurations:Distribution"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }
end

if _OPTIONS["tests"] then
    -- EmuTests Executable
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
            "Emu/public"
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
            runtime "Debug"
            symbols "On"
            defines { "DEBUG" }

        filter "configurations:Release"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }

        filter "configurations:Distribution"
            runtime "Release"
            optimize "On"
            defines { "NDEBUG" }
end