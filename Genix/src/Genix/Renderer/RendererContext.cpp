#include "gxpch.h"
#include "RendererContext.h"

#include "Renderer.h"
#include "Genix/Core.h"
#include "Genix/Platform/OpenGL/OpenGLContext.h"

Scope<RendererContext> RendererContext::Create(void* window)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	case RendererAPI::API::DirectX: ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
