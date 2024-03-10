#include "gxpch.h"
#include "OpenGLContext.h"

#include "Genix/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	ASSERT_CORE(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT_CORE(status, "Failed to initialize Glad!")

	LOG_CORE_INFO("OpenGL Info:");
	LOG_CORE_INFO("  Vendor: {0}",  (const char*)glGetString(GL_VENDOR));
	LOG_CORE_INFO("  Renderer: {0}",  (const char*)glGetString(GL_RENDERER));
	LOG_CORE_INFO("  Version: {0}",  (const char*)glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
