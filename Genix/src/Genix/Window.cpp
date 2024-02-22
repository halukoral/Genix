#include "gxpch.h"
#include "Window.h"

static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowAttributes& attributes)
{
	return new Window(attributes);
}
	
Window::Window(const WindowAttributes& attributes)
{
	Init(attributes);
}

Window::~Window()
{
	Shutdown();
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
	
void Window::Init(const WindowAttributes& attributes)
{
	m_Data.Title = attributes.Title;
	m_Data.Width = attributes.Width;
	m_Data.Height = attributes.Height;

	LOG_CORE_INFO("Creating window {0} ({1}, {2})", attributes.Title, attributes.Width, attributes.Height);

	if (!s_GLFWInitialized)
	{
		[[maybe_unused]] int success = glfwInit();
		GX_CORE_ASSERT(success, "Could not intialize GLFW!");

		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)attributes.Width, (int)attributes.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);	
}
	
void Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	m_Data.VSync = enabled;
}

bool Window::IsVSyncEnabled() const
{
	return m_Data.VSync;
}