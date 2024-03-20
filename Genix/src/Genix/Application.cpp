#include "gxpch.h"
#include "Application.h"

#include "Log.h"

#include "Common/PrimitiveTypes.h"
#include "Common/TimeStep.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Layer/Layer.h"
#include "Renderer/Renderer.h"

#include <glfw/glfw3.h>

#include "Events/KeyEvent.h"
#include "Input/KeyCodes.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT_CORE(!s_Instance, "Application already exists!")
	s_Instance = this;
	
	m_Window = Window::Create();
	m_Window->SetEventCallback(GX_BIND(Application::OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
	Renderer::Shutdown();
}

void Application::Run()
{
	while (m_Running)
	{
		const float time = (float)glfwGetTime();
		const TimeStep timeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timeStep);
			}
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}
		m_ImGuiLayer->End();
		
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(GX_BIND(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(GX_BIND(Application::OnWindowResize));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		(*it)->OnEvent(e);
		if (e.GetHandled() == true)
		{
			 break;
		}
	}
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	return false;
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return false;
}
