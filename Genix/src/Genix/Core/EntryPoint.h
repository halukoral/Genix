#pragma once
#include "Genix/Core/Application.h"

extern Application* CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Log::Init();
	
	const auto app = CreateApplication({ argc, argv });
	app->Run();
	delete app;
}
