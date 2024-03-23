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
IncludeDir["GLFW"] = "Genix/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "Genix/ThirdParty/glad/include"
IncludeDir["glm"] = "Genix/ThirdParty/glm"
IncludeDir["assimp"] = "Genix/ThirdParty/assimp/include"
IncludeDir["spdlog"] = "Genix/ThirdParty/spdlog/include"
IncludeDir["ImGui"] = "Genix/ThirdParty/imgui"
IncludeDir["stb_image"] = "Genix/ThirdParty/stb_image"

group "Dependencies"
    include "Genix/ThirdParty/glad"
group ""


project "Genix"
    location "Genix"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        
    pchheader "gxpch.h"
    pchsource "Genix/src/gxpch.cpp"
       
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/ThirdParty/stb_image/**.h",
        "%{prj.name}/ThirdParty/stb_image/**.cpp",
        "%{prj.name}/ThirdParty/glm/glm/**.hpp",
        "%{prj.name}/ThirdParty/glm/glm/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }
         
    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.stb_image}"
	}

    libdirs 
    {
        "%{prj.name}/Libs"
    }

	links 
	{ 
        "opengl32.lib",
		"GLFW",
		"Glad",
        "ImGui"
	}
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
			"GX_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
        
    filter "configurations:Debug"
        defines "GX_DEBUG"
        runtime "Debug"
        symbols "on"
        links { "assimp-vc143-mtd.lib" }
        
    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        optimize "on"
        links { "assimp-vc143-mt.lib" }

            
    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        optimize "on"
        links { "assimp-vc143-mt.lib" }

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/Assets/**"
    }
    
    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}",
        "Genix/src",
        "Genix/ThirdParty"
    }

    links
    {
        "Genix"
    }
    
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "GX_DEBUG"
        runtime "Debug"
        symbols "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Debug \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Release \"../bin/" .. outputdir .. "/Sandbox/\"")
        }
            
    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Release \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

project "GenixED"
    location "GenixED"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/Assets/**"
    }

    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}",
        "Genix/src",
        "Genix/ThirdParty"
    }

    links
    {
        "Genix"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "GX_DEBUG"
        runtime "Debug"
        symbols "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Debug \"../bin/" .. outputdir .. "/GenixED/\"")
        }

    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Release \"../bin/" .. outputdir .. "/GenixED/\"")
        }

    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} ../DLL/Release \"../bin/" .. outputdir .. "/GenixED/\"")
        }

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
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"
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

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
        symbols "off"