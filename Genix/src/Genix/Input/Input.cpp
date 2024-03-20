#include "gxpch.h"
#include "Input.h"

#ifdef  GX_PLATFORM_WINDOWS
	#include "Genix/Platform/GLFW/Input_GLFW.h"
#endif

Scope<Input> Input::s_Instance = Input::Create();

Scope<Input> Input::Create()
{
#ifdef GX_PLATFORM_WINDOWS
	return CreateScope<Input_GLFW>();
#else
	ASSERT_CORE(false, "Unknown platform!")
	return nullptr;
#endif
}
