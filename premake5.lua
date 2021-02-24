workspace "RyEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations { "Debug", "Release"}
		shadermodel "6.0"
		shaderobjectfileoutput ""
		shaderentry ""

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RyEngine"
	location "RyEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/shaders/**.hlsl",
		"%{prj.name}/assets/shaders/**.hlsli"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"bin/" .. outputdir .. "/%{prj.name}"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RE_DLL_BUILD",
			"RE_WINDOWS",
			"_USE_MATH_DEFINES"
		}

		links
		{
			"d3dcompiler",
			"dxguid",
			"D3D12",
			"dxgi"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"

	filter "files:**.hlsl"
		flags "ExcludeFromBuild"
		shadermodel "6.0"
		shadervariablename "g_s%{file.basename}"
		shaderheaderfileoutput "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/CompiledShaders/%{file.basename}.h"

	filter "files:**VS.hlsl"
		removeflags "ExcludeFromBuild"
		shadertype "Vertex"
		shaderentry "VS"

	filter "files:**PS.hlsl"
		removeflags "ExcludeFromBuild"
		shadertype "Pixel"
		shaderentry "PS"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"RyEngine/src",
		"RyEngine/vendor/spdlog/include"
	}

	links
	{
		"RyEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RE_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"