#pragma once
#include "Genix/Renderer/RendererContext.h"

struct GLFWwindow;

class OpenGLContext : public RendererContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);

	void Init() override;
	void SwapBuffers() override;

private:
	GLFWwindow* m_WindowHandle;	
};
