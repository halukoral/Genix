#pragma once

struct GLFWwindow;

class RendererContext
{
public:
	RendererContext(GLFWwindow* windowHandle);
	virtual ~RendererContext() = default;
	
	void Init();
	void SwapBuffers();
	void Shutdown() {}

	static Scope<RendererContext> Create(void* window);

private:
	GLFWwindow* m_WindowHandle;	
};
