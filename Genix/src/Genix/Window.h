#pragma once

#include "gxpch.h"

#include "Events/Event.h"
#include "Genix/Core.h"
#include <GLFW/glfw3.h>

using EventCallbackFn = std::function<void(Event&)>;

struct WindowAttributes
{
	bool VSync;
	std::string Title;
	unsigned int Width;
	unsigned int Height;
	EventCallbackFn EventCallback;

	WindowAttributes(bool vSync = true,
					 const std::string& title = "Genix Engine",
					 unsigned int width = 1920,
					 unsigned int height = 1080)
		: Title(title), Width(width), Height(height), VSync(vSync)
	{
	}
};

class GENIX_API Window
{
public:
		
	Window(const WindowAttributes& attributes);
	~Window();

	void OnUpdate();

	unsigned int GetWidth() const { return m_Data.Width; }
	unsigned int GetHeight() const { return m_Data.Height; }

	GLFWwindow* GetNativeWindow() const { return m_Window; }
		
	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled);
	bool IsVSyncEnabled() const;

	static Window* Create(const WindowAttributes& attributes = WindowAttributes());
		
private:
	void Init(const WindowAttributes& attributes);
	void Shutdown();

	WindowAttributes m_Data;		
	GLFWwindow* m_Window;
};
