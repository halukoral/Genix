#include "gxpch.h"
#include "Genix/Core/Window.h"

#ifdef GX_PLATFORM_WINDOWS
	#include "Genix/Platform/Windows/WindowsWindow.h"
#endif

Scope<Window> Window::Create(const WindowAttributes& props)
{
#ifdef GX_PLATFORM_WINDOWS
	return CreateScope<WindowsWindow>(props);
#else
	ASSERT_CORE(false, "Unknown platform!");
	return nullptr;
#endif
}
