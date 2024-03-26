project "Genix"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
        
    pchheader "gxpch.h"
    pchsource "src/gxpch.cpp"
       
    files
    {
        "src/**.h",
        "src/**.cpp",
        "ThirdParty/stb_image/**.h",
        "ThirdParty/stb_image/**.cpp",
        "ThirdParty/glm/glm/**.hpp",
        "ThirdParty/glm/glm/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }
            
    includedirs
    {
      "src",
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
        "Libs"
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
