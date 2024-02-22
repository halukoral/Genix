#pragma once
#include "Application.h"

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Log::Init();
	LOG_CORE_INFO("Creating window");

	auto app = CreateApplication();
	app->Run();
	delete app;
}
