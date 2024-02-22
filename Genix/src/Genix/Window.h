#pragma once

#include "gxpch.h"

#include "Events/Event.h"
#include "Genix/Core.h"
#include <GLFW/glfw3.h>

struct WindowAttributes
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowAttributes(const std::string& title = "Genix Engine",
				unsigned int width = 1920,
				unsigned int height = 1080)
		: Title(title), Width(width), Height(height)
	{
	}
};

class GENIX_API Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;
		
	Window(const WindowAttributes& attributes);
	~Window();

	void OnUpdate();

	unsigned int GetWidth() const { return m_Data.Width; }
	unsigned int GetHeight() const { return m_Data.Height; }

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled);
	bool IsVSyncEnabled() const;

	static Window* Create(const WindowAttributes& attributes = WindowAttributes());
		
private:
	virtual void Init(const WindowAttributes& attributes);
	virtual void Shutdown();

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;		
	GLFWwindow* m_Window;
};
