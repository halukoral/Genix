#pragma once

#include "gxpch.h"
#include "Genix/Core.h"
#include "Events/Event.h"

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

// Interface representing a desktop system based Window
class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	static Scope<Window> Create(const WindowAttributes& props = WindowAttributes());

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	unsigned int GetWidth() const { return m_Data.Width; }
	unsigned int GetHeight() const { return m_Data.Height; }

	const WindowAttributes& GetWindowAttributes() const { return m_Data; }
	
	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void* GetNativeWindow() const = 0;

protected:
	WindowAttributes m_Data;		
};
