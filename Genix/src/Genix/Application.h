#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "Window.h"

class IndexBuffer;
class VertexBuffer;
class Shader;
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
	bool OnWindowClose(WindowCloseEvent& e);

	bool m_Running = true;
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;

	unsigned int m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	std::string vertexSrc;
	std::string fragmentSrc;
	
	static Application* s_Instance;
};

// For CLIENT
Application* CreateApplication();