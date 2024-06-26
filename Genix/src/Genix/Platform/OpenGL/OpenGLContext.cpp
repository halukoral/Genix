﻿#include "gxpch.h"
#include "Genix/Platform/OpenGL/OpenGLContext.h"

#include "Genix/Core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	ASSERT_CORE(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
	GX_PROFILE_FUNCTION();

	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT_CORE(status, "Failed to initialize Glad!")

	LOG_CORE_INFO("OpenGL Info:");
	LOG_CORE_INFO("  Vendor: {0}",  (const char*)glGetString(GL_VENDOR));
	LOG_CORE_INFO("  Renderer: {0}",  (const char*)glGetString(GL_RENDERER));
	LOG_CORE_INFO("  Version: {0}",  (const char*)glGetString(GL_VERSION));

#ifdef GX_ENABLE_ASSERTS
	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

	ASSERT_CORE(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Genix requires at least OpenGL version 4.5!")
#endif
}

void OpenGLContext::SwapBuffers()
{
	GX_PROFILE_FUNCTION();

	glfwSwapBuffers(m_WindowHandle);
}
