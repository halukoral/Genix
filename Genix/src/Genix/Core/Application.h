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

struct ApplicationCommandLineArgs
{
	int Count = 0;
	char** Args = nullptr;

	const char* operator[](int index) const
	{
		ASSERT_CORE(index < Count);
		return Args[index];
	}
};

struct ApplicationSpecification
{
	std::string Name = "Genix Application";
	std::string WorkingDirectory;
	ApplicationCommandLineArgs CommandLineArgs;
};

class Application
{
public:
	Application(const ApplicationSpecification& specification);
	virtual ~Application();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	void OnClose() { m_Running = false; }

	void Close();
	
	Window& GetWindow() const { return *m_Window; }
	ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
	
	static Application& Get() { return *s_Instance; }

	const ApplicationSpecification& GetSpecification() const { return m_Specification; }
	
private:
	void Run();

	bool OnWindowResize(WindowResizeEvent& e);
	bool OnWindowClose(WindowCloseEvent& e);

private:
	ApplicationSpecification m_Specification;
	
	bool m_Minimized = false;
	bool m_Running = true;
	
	float m_LastFrameTime = 0.0f;
	
	Scope<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	LayerStack m_LayerStack;
	
	static Application* s_Instance;
	friend int ::main(int argc, char** argv);
};

// For CLIENT
Application* CreateApplication(ApplicationCommandLineArgs args);