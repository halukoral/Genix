#include "gxpch.h"
#include "Window.h"

#ifdef GX_PLATFORM_WINDOWS
	#include "Platform/GLFW/Window_GLFW.h"
#endif

Scope<Window> Window::Create(const WindowAttributes& props)
{
#ifdef GX_PLATFORM_WINDOWS
	return CreateScope<Window_GLFW>(props);
#else
	ASSERT_CORE(false, "Unknown platform!");
	return nullptr;
#endif
}
