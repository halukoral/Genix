#pragma once

#include "Genix/Core/Core.h"
#include "Genix/Events/Event.h"

using EventCallbackFn = std::function<void(Event&)>;

struct WindowAttributes
{
	bool VSync;
	std::string Title;
	uint32 Width;
	uint32 Height;
	EventCallbackFn EventCallback;

	WindowAttributes(const std::string& title = "Genix Engine",
					 bool vSync = true,
					 uint32 width = 2560,
					 uint32 height = 1440)
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

	uint32 GetWidth() const { return m_Data.Width; }
	uint32 GetHeight() const { return m_Data.Height; }

	const WindowAttributes& GetWindowAttributes() const { return m_Data; }
	
	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void EnableCursor() = 0;
	virtual void DisableCursor() = 0;
	
	virtual void* GetNativeWindow() const = 0;

protected:
	WindowAttributes m_Data;		
};
