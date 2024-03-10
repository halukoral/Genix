#include "gxpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "RendererAPI.h"
#include "Genix/Platform/OpenGL/OpenGLVertexArray.h"

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    ASSERT_CORE(false, "RendererAPI::None is currently not supported!") return nullptr;
	case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
