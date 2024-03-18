#include "gxpch.h"
#include "RendererAPI.h"

#include "Genix/Platform/OpenGL/OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
	switch (s_API)
	{
	case API::None:		ASSERT_CORE(false, "RendererAPI::None is currently not supported!") return nullptr;
	case API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
	case API::DirectX:  ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!") return nullptr;
	
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}