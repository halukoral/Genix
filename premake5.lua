workspace "Genix"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"    

IncludeDir = {}
IncludeDir["GLFW"] = "Genix/ThirdParty/GLFW"
IncludeDir["Glad"] = "Genix/ThirdParty/glad/include"
IncludeDir["SpdLog"] = "Genix/ThirdParty/spdlog/include"
IncludeDir["ImGui"] = "Genix/ThirdParty/imgui"

include "Genix/ThirdParty/glad"

project "Genix"
    location "Genix"
    kind "SharedLib"
    language "C++"    

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        
    pchheader "gxpch.h"
    pchsource "Genix/src/gxpch.cpp"
       
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
        
    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.SpdLog}",
        "%{IncludeDir.ImGui}"
	}

    libdirs 
    {
        "%{prj.name}/Libs"
    }

	links 
	{ 
        "opengl32.lib",
		"glfw3.lib",
		"dwmapi.lib",
		"Glad",
        "ImGui"
	}
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "GX_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
        
    filter "configurations:Debug"
        defines "GX_DEBUG"
        runtime "Debug"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        buildoptions "/MD"
        optimize "On"
            
    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"  
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "Genix/ThirdParty",
        "Genix/ThirdParty/GLFW",
        "Genix/ThirdParty/spdlog/include",
        "Genix/src"
    }
    
    links
    {
        "Genix"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "GX_DEBUG"
        runtime "Debug"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        buildoptions "/MD"
        optimize "On"
            
    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        buildoptions "/MD"
        optimize "On"

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