#include "gxpch.h"
#include "Window_GLFW.h"

#include "Genix/Events/ApplicationEvent.h"
#include "Genix/Events/KeyEvent.h"
#include "Genix/Events/MouseEvent.h"

#include "Genix/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static uint8_t s_GLFWWindowCount = 0;

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
			MouseButtonPressedEvent event((MouseCode)button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event((MouseCode)button);
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
		KeyPressedEvent event((KeyCode)(KeyCode)key, 0);
		data.EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent event((KeyCode)key);
		data.EventCallback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent event((KeyCode)key, 1);
		data.EventCallback(event);
		break;
	}
	default:
		break;
	}	
}

void CharCallback(GLFWwindow* window, unsigned int keycode)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	KeyTypedEvent event((KeyCode)keycode);
	data.EventCallback(event);
}

Window_GLFW::Window_GLFW(const WindowAttributes& attributes)
{
	Init(attributes);
}

Window_GLFW::~Window_GLFW()
{
	Shutdown();
}

void Window_GLFW::SetVSync(bool enabled)
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

bool Window_GLFW::IsVSync() const
{
	return m_Data.VSync;
}

void Window_GLFW::Init(const WindowAttributes& attributes)
{
	m_Data.Title = attributes.Title;
	m_Data.Width = attributes.Width;
	m_Data.Height = attributes.Height;

	LOG_CORE_INFO("Creating window {0} ({1}, {2})", attributes.Title, attributes.Width, attributes.Height);

	if (s_GLFWWindowCount == 0)
	{
		LOG_CORE_INFO("Initializing GLFW");
		int success = glfwInit();
		ASSERT_CORE(success, "Could not intialize GLFW!")

		glfwSetErrorCallback(GLFWErrorCallback);
	}

	m_Window = glfwCreateWindow((int)attributes.Width, (int)attributes.Height, m_Data.Title.c_str(), nullptr, nullptr);
	++s_GLFWWindowCount;
	
	m_Context = CreateScope<OpenGLContext>(m_Window);
	m_Context->Init();
	
	SetVSync(true);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	
	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, FramebufferSizeCallback);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);

	glfwSetKeyCallback(m_Window, KeyCallback);
	glfwSetCharCallback(m_Window, CharCallback);
	
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	glfwSetScrollCallback(m_Window, ScrollCallback);
	glfwSetCursorPosCallback(m_Window, MouseCallback);
}

void Window_GLFW::OnUpdate()
{
	glfwPollEvents();
	m_Context->SwapBuffers();
}

void Window_GLFW::Shutdown()
{
	glfwDestroyWindow(m_Window);
	
	if (--s_GLFWWindowCount == 0)
	{
		LOG_CORE_INFO("Terminating GLFW");
		glfwTerminate();	
	}
}
