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
        "%{prj.name}/ThirdParty/spdlog/include",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
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