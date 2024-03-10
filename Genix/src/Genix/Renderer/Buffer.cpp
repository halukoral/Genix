#include "gxpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLVertexBuffer(vertices, size);
	case RendererAPI::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLIndexBuffer(indices, size);
	case RendererAPI::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
