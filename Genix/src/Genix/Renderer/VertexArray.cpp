#include "gxpch.h"
#include "Genix/Renderer/VertexArray.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/RendererAPI.h"
#include "Genix/Platform/OpenGL/OpenGLVertexArray.h"

Ref<VertexArray> VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLVertexArray>();

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
