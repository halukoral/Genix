#pragma once

#include "Genix/Core/Core.h"
#include "Genix/Events/Event.h"

using EventCallbackFn = std::function<void(Event&)>;
class RendererContext;
struct GLFWwindow;

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

	Window(const WindowAttributes& attributes);
	virtual ~Window();

	void OnUpdate();

	uint32 GetWidth() const { return m_Data.Width; }
	uint32 GetHeight() const { return m_Data.Height; }

	const WindowAttributes& GetWindowAttributes() const { return m_Data; }
	
	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled);
	bool IsVSync() const;

	void EnableCursor();
	void DisableCursor();
	
	void* GetNativeWindow() const { return m_Window; }

protected:
	WindowAttributes m_Data;
	
private:
	virtual void Init(const WindowAttributes& attributes);
	virtual void Shutdown();

	GLFWwindow* m_Window;
	Scope<RendererContext> m_Context;
};
