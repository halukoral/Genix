#pragma once

#include "Core.h"
#include "Window.h"
#include "Layer/LayerStack.h"

class WindowResizeEvent;
class IndexBuffer;
class VertexBuffer;
class ImGuiLayer;
class Layer;
class WindowCloseEvent;

class GENIX_API Application
{
public:
	Application();
	virtual ~Application() = default;

	void Run();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	Window& GetWindow() const { return *m_Window; }

	static Application& Get() { return *s_Instance; }
	
private:
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);

	bool m_Minimized = false;
	bool m_Running = true;
	
	float m_LastFrameTime = 0.0f;
	
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	
	static Application* s_Instance;
};

// For CLIENT
Application* CreateApplication();