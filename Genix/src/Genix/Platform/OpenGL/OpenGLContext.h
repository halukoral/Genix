#pragma once
#include "Genix/Renderer/RendererContext.h"

struct GLFWwindow;

class GENIX_API OpenGLContext : public RendererContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);

	void Init() override;
	void SwapBuffers() override;

private:
	GLFWwindow* m_WindowHandle;	
};
