#pragma once

#include "Genix/Core/Window.h"

class RendererContext;
struct GLFWwindow;

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowAttributes& attributes);
	~WindowsWindow() override;

	void OnUpdate() override;

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	void EnableCursor() override;
	void DisableCursor() override;
	
	void* GetNativeWindow() const override { return m_Window; }
	
private:
	virtual void Init(const WindowAttributes& attributes);
	virtual void Shutdown();

	GLFWwindow* m_Window;
	Scope<RendererContext> m_Context;
};
