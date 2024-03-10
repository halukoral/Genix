#include "gxpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		GX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLVertexBuffer(vertices, size);
	case RendererAPI::DirectX:
		GX_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	GX_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		GX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLIndexBuffer(indices, size);
	case RendererAPI::DirectX:
		GX_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	GX_CORE_ASSERT(false, "Unknown RendererAPI!")
	return nullptr;
}
