#include "gxpch.h"
#include "Genix/Renderer/RendererContext.h"
#include "Genix/Renderer/Renderer.h"

#include "Genix/Core/Core.h"
#include "Genix/Core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Scope<RendererContext> RendererContext::Create(void* window)
{
	return CreateScope<RendererContext>(static_cast<GLFWwindow*>(window));
}

RendererContext::RendererContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	ASSERT_CORE(windowHandle, "Window handle is null!")
}

void RendererContext::Init()
{
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

void RendererContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
