﻿#include "gxpch.h"
#include "Genix/Platform/Windows/WindowsWindow.h"
#include "Genix/Platform/OpenGL/OpenGLContext.h"

#include "Genix/Events/ApplicationEvent.h"
#include "Genix/Events/KeyEvent.h"
#include "Genix/Events/MouseEvent.h"

#include "Genix/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Genix/Platform/Vulkan/VulkanContext.h"

static uint8 s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

void FramebufferSizeCallback(GLFWwindow* window, int32 width, int32 height)
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

void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods)
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

void KeyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
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
		KeyPressedEvent event((KeyCode)key, true);
		data.EventCallback(event);
		break;
	}
	default:
		break;
	}	
}

void CharCallback(GLFWwindow* window, uint32 keycode)
{
	WindowAttributes& data = *(WindowAttributes*)glfwGetWindowUserPointer(window);

	KeyTypedEvent event((KeyCode)keycode);
	data.EventCallback(event);
}

WindowsWindow::WindowsWindow(const WindowAttributes& attributes)
{
	GX_PROFILE_FUNCTION();

	Init(attributes);
}

WindowsWindow::~WindowsWindow()
{
	GX_PROFILE_FUNCTION();

	Shutdown();
}

void WindowsWindow::SetVSync(bool enabled)
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

bool WindowsWindow::IsVSync() const
{
	return m_Data.VSync;
}

void WindowsWindow::EnableCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WindowsWindow::DisableCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowsWindow::Init(const WindowAttributes& attributes)
{
	m_Data.Title = attributes.Title;
	m_Data.Width = attributes.Width;
	m_Data.Height = attributes.Height;

	LOG_CORE_INFO("Creating window {0} ({1}, {2})", attributes.Title, attributes.Width, attributes.Height);

	if (s_GLFWWindowCount == 0)
	{
		GX_PROFILE_SCOPE("glfwInit");
		
		LOG_CORE_INFO("Initializing GLFW");
		int success = glfwInit();
		ASSERT_CORE(success, "Could not intialize GLFW!")

		glfwSetErrorCallback(GLFWErrorCallback);
	}

#if defined(GX_DEBUG)
	if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	}
#endif


	if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
	{
		if (!glfwVulkanSupported())
		{
			glfwTerminate();
			ASSERT_CORE(false, "Vulkan is not supported");
			return;
		}
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	
	{
		GX_PROFILE_SCOPE("glfwCreateWindow");
		m_Window = glfwCreateWindow((int32)attributes.Width, (int32)attributes.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;		
	}

	if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
	{
		m_Context = OpenGLContext::Create(m_Window);
	}
	else
	{
		m_Context = VulkanContext::Create(m_Window);
	}
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

	// Tell GLFW to capture our mouse
	//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowsWindow::OnUpdate()
{
	GX_PROFILE_FUNCTION();

	glfwPollEvents();
	m_Context->SwapBuffers();
}

void WindowsWindow::Shutdown()
{
	GX_PROFILE_FUNCTION();

	m_Context->Shutdown();
	--s_GLFWWindowCount;
	glfwDestroyWindow(m_Window);

	if (s_GLFWWindowCount == 0)
	{
		LOG_CORE_INFO("Terminating GLFW");
		glfwTerminate();	
	}
}