#pragma once
#include "Genix/Core/Application.h"

#ifdef GX_PLATFORM_WINDOWS

extern Application* CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Log::Init();
	
	GX_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	const auto app = CreateApplication({ argc, argv });
	GX_PROFILE_END_SESSION();

	GX_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->Run();
	GX_PROFILE_END_SESSION();

	GX_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	GX_PROFILE_END_SESSION();
}

#endif