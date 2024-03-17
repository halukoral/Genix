#pragma once

#include "Core.h"

#include "Layer/LayerStack.h"
#include "Window.h"

#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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
	
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	std::shared_ptr<Shader> m_BlueShader;
	std::shared_ptr<VertexArray> m_SquareVA;

	Camera m_Camera;
	
	static Application* s_Instance;
};

// For CLIENT
Application* CreateApplication();