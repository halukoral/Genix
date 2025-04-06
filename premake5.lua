include "./ThirdParty/premake/premake_customization/solution_items.lua"

workspace "Genix"
    architecture "x86_64"
    startproject "GenixED"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"    

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Genix/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Genix/ThirdParty/glad/include"
IncludeDir["glm"] = "%{wks.location}/Genix/ThirdParty/glm"
IncludeDir["assimp"] = "%{wks.location}/Genix/ThirdParty/assimp/include"
IncludeDir["spdlog"] = "%{wks.location}/Genix/ThirdParty/spdlog/include"
IncludeDir["ImGui"] = "%{wks.location}/Genix/ThirdParty/imgui"
IncludeDir["stb_image"] = "%{wks.location}/Genix/ThirdParty/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Genix/ThirdParty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Genix/ThirdParty/ImGuizmo"

group "Dependencies"
    include "Genix/ThirdParty/glad"
group ""

include "Genix"
include "GenixED"
include "Sandbox"

project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Genix/ThirdParty/imgui/imconfig.h",
		"Genix/ThirdParty/imgui/imgui.h",
		"Genix/ThirdParty/imgui/imgui.cpp",
		"Genix/ThirdParty/imgui/imgui_draw.cpp",
		"Genix/ThirdParty/imgui/imgui_internal.h",
		"Genix/ThirdParty/imgui/imgui_widgets.cpp",
		"Genix/ThirdParty/imgui/imgui_tables.cpp",
		"Genix/ThirdParty/imgui/imstb_rectpack.h",
		"Genix/ThirdParty/imgui/imstb_textedit.h",
		"Genix/ThirdParty/imgui/imstb_truetype.h",
		"Genix/ThirdParty/imgui/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "GLFW"
	kind "StaticLib"
	language "C"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    GLFWDir = "Genix/ThirdParty/GLFW"

	files
	{
		"%{GLFWDir}/include/GLFW/glfw3.h",
		"%{GLFWDir}/include/GLFW/glfw3native.h",
		"%{GLFWDir}/src/glfw_config.h",
		"%{GLFWDir}/src/context.c",
		"%{GLFWDir}/src/init.c",
		"%{GLFWDir}/src/input.c",
		"%{GLFWDir}/src/monitor.c",

		"%{GLFWDir}/src/null_init.c",
		"%{GLFWDir}/src/null_joystick.c",
		"%{GLFWDir}/src/null_monitor.c",
		"%{GLFWDir}/src/null_window.c",

		"%{GLFWDir}/src/platform.c",
		"%{GLFWDir}/src/vulkan.c",
		"%{GLFWDir}/src/window.c",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"%{GLFWDir}/src/win32_init.c",
			"%{GLFWDir}/src/win32_joystick.c",
			"%{GLFWDir}/src/win32_module.c",
			"%{GLFWDir}/src/win32_monitor.c",
			"%{GLFWDir}/src/win32_time.c",
			"%{GLFWDir}/src/win32_thread.c",
			"%{GLFWDir}/src/win32_window.c",
			"%{GLFWDir}/src/wgl_context.c",
			"%{GLFWDir}/src/egl_context.c",
			"%{GLFWDir}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"



project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    yamlDir = "Genix/ThirdParty/yaml-cpp"

	files
	{
		"%{yamlDir}/src/**.h",
		"%{yamlDir}/src/**.cpp",
		
		"%{yamlDir}/include/**.h"
	}

	includedirs
	{
		"%{yamlDir}/include"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"