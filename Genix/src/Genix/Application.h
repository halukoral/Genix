#pragma once

#include "Core.h"
#include "Window.h"
#include "Layer/LayerStack.h"

class ImGuiLayer;
class IndexBuffer;
class Layer;
class VertexBuffer;
class WindowCloseEvent;
class WindowResizeEvent;

int main(int argc, char** argv);

class Application
{
public:
	Application();
	virtual ~Application();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	void OnClose() { m_Running = false; }
	
	Window& GetWindow() const { return *m_Window; }

	static Application& Get() { return *s_Instance; }
	
private:
	void Run();

	bool OnWindowResize(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);

	bool m_Minimized = false;
	bool m_Running = true;
	
	float m_LastFrameTime = 0.0f;
	
	LayerStack m_LayerStack;
	Scope<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	
	static Application* s_Instance;
	friend int ::main(int argc, char** argv);
};

// For CLIENT
Application* CreateApplication();