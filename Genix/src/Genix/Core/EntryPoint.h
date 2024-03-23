#pragma once
#include "Genix/Core/Core.h"
#include "Genix/Core/Application.h"

#ifdef GX_PLATFORM_WINDOWS

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Log::Init();
	LOG_CORE_INFO("Creating window");
	auto app = CreateApplication();
	app->Run();
	delete app;
}

#endif