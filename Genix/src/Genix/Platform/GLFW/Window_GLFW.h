#pragma once

#include "Genix/Window.h"

class GLFWwindow;

class GENIX_API Window_GLFW : public Window
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
};
