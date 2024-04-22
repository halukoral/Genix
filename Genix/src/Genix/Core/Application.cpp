#include "gxpch.h"
#include "Genix/Core/Application.h"
#include "Genix/Core/Log.h"

#include "Genix/Common/TimeStep.h"
#include "Genix/Events/ApplicationEvent.h"
#include "Genix/ImGui/ImGuiLayer.h"
#include "Genix/Layer/Layer.h"
#include "Genix/Renderer/Renderer.h"
#include "Genix/Utils/PlatformUtils.h"

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
{
	GX_PROFILE_FUNCTION();
	ASSERT_CORE(!s_Instance, "Application already exists!")
	s_Instance = this;
	
	// Set working directory here
	if (!m_Specification.WorkingDirectory.empty())
	{
		std::filesystem::current_path(m_Specification.WorkingDirectory);
	}

	m_Window = Window::Create(WindowAttributes(m_Specification.Name));
	m_Window->SetEventCallback(GX_BIND(Application::OnEvent));

	Renderer::Init();
	
	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
	GX_PROFILE_FUNCTION();

	Renderer::Shutdown();
}

void Application::Run()
{
	GX_PROFILE_FUNCTION();

	while (m_Running)
	{
		GX_PROFILE_SCOPE("RunLoop");
		
		const float time = Time::GetTime();
		const TimeStep deltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			{
				GX_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}

			m_ImGuiLayer->Begin();
			{
				GX_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();
		}		
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	GX_PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(GX_BIND(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(GX_BIND(Application::OnWindowResize));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if (e.GetHandled() == true)
		{
			 break;
		}
		(*it)->OnEvent(e);
	}
}

void Application::PushLayer(Layer* layer)
{
	GX_PROFILE_FUNCTION();

	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
	GX_PROFILE_FUNCTION();

	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

void Application::Close()
{
	m_Running = false;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	GX_PROFILE_FUNCTION();

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
