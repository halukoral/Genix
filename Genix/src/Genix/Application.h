#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"

class Layer;
class WindowCloseEvent;

class GENIX_API Application
{
public:
	Application();
	virtual ~Application();

	void Run();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
	
private:
	bool OnWindowClose(WindowCloseEvent& e);

	bool m_Running = true;
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;
};

// For CLIENT
Application* CreateApplication();