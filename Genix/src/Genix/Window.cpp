#include "gxpch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glad/glad.h>

static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowAttributes& attributes)
{
	return new Window(attributes);
}

static void GLFWErrorCallback(int error, const char* description)
{
	LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);
	data.Width = width;
	data.Height = height;

	WindowResizeEvent event(width, height);
	data.EventCallback(event);	
}

void WindowCloseCallback(GLFWwindow* window)
{
	const WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);
	WindowCloseEvent event;
	data.EventCallback(event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	const WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
	{
		MouseButtonPressedEvent event(button);
		data.EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		MouseButtonReleasedEvent event(button);
		data.EventCallback(event);
		break;
	}
	}	
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	MouseMovedEvent event((float)xPos, (float)yPos);
	data.EventCallback(event);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	MouseScrolledEvent event((float)xOffset, (float)yOffset);
	data.EventCallback(event);	
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	switch (action)
	{
	case GLFW_PRESS:
	{
		KeyPressedEvent event(key, 0);
		data.EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent event(key);
		data.EventCallback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent event(key, 1);
		data.EventCallback(event);
		break;
	}
	default:
		break;
	}	
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

		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)attributes.Width, (int)attributes.Height, m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GX_CORE_ASSERT(status, "Failed to initialize Glad!");
	
	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, FramebufferSizeCallback);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);

	glfwSetKeyCallback(m_Window, KeyCallback);

	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	glfwSetScrollCallback(m_Window, ScrollCallback);
	glfwSetCursorPosCallback(m_Window, MouseCallback);
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