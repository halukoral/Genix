#pragma once

#include "Core.h"
#include "Window.h"

class WindowCloseEvent;

class GENIX_API Application
{
public:
	Application();
	virtual ~Application();

	void Run();
	void OnEvent(Event& e);
	
private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
};

// For CLIENT
Application* CreateApplication();