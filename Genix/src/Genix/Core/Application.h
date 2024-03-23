#pragma once

#include "Genix/Core/Core.h"
#include "Genix/Core/Window.h"
#include "Genix/Layer/LayerStack.h"

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
	Application(const std::string& name = "Genix App");
	virtual ~Application();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	void OnClose() { m_Running = false; }

	void Close();
	
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