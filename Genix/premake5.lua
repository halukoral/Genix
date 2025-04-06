project "Genix"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
        "ThirdParty/glm/glm/**.inl",
        "ThirdParty/ImGuizmo/ImGuizmo.h",
        "ThirdParty/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
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
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.VulkanSDK}"
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
        "ImGui",
        "yaml-cpp",
        "%{Library.Vulkan}",
	}
    
    filter "files:ThirdParty/ImGuizmo/**.cpp"
	    flags
        {
            "NoPCH"
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
        links 
        { 
            "assimp-vc143-mtd.lib",
            "%{Library.ShaderC_Debug}",
            "%{Library.SPIRV_Cross_Debug}",
            "%{Library.SPIRV_Cross_GLSL_Debug}"
        }
        
    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        optimize "on"
        links 
        { 
            "assimp-vc143-mt.lib",
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }

            
    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        optimize "on"
        links 
        { 
            "assimp-vc143-mt.lib",
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }
