project "GenixED"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "Assets/**"
    }

    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.ImGuizmo}",
        "%{wks.location}/Genix/src",
        "%{wks.location}/Genix/ThirdParty"
    }

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
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
            ("{COPY} %{wks.location}/DLL/Debug \"%{wks.location}/bin/" .. outputdir .. "/GenixED/\"")
        }

    filter "configurations:Release"
        defines "GX_RELEASE"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} %{wks.location}/DLL/Release \"%{wks.location}/bin/" .. outputdir .. "/GenixED/\"")
        }

    filter "configurations:Dist"
        defines "GX_DIST"
        runtime "Release"
        optimize "on"
        postbuildcommands
        {
            ("{COPY} %{wks.location}/DLL/Release \"%{wks.location}/bin/" .. outputdir .. "/GenixED/\"")
        }
