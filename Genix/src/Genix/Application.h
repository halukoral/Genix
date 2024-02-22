#pragma once

#include "Core.h"
#include "Window.h"

class GENIX_API Application
{
public:
	Application();
	virtual ~Application();

	void Run();
	
private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
};

// For CLIENT
Application* CreateApplication();