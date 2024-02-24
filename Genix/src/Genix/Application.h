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

	Window& GetWindow() const { return *m_Window; }

	static Application& Get() { return *s_Instance; }
	
private:
	bool OnWindowClose(WindowCloseEvent& e);

	bool m_Running = true;
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;

	static Application* s_Instance;
};

// For CLIENT
Application* CreateApplication();