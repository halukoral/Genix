#pragma once

#include "Genix/Core/Window.h"

class RendererContext;
struct GLFWwindow;

class Window_GLFW : public Window
{
public:
	Window_GLFW(const WindowAttributes& attributes);
	~Window_GLFW() override;

	void OnUpdate() override;

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	void* GetNativeWindow() const override { return m_Window; }
	
private:
	virtual void Init(const WindowAttributes& attributes);
	virtual void Shutdown();

	GLFWwindow* m_Window;
	Scope<RendererContext> m_Context;
};
