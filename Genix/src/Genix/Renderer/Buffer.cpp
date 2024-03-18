#include "gxpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "RendererAPI.h"
#include "Genix/Platform/OpenGL/OpenGLBuffer.h"

uint32 BufferElement::GetComponentCount() const
{
	switch (Type)
	{
	case ShaderDataType::Float:   return 1;
	case ShaderDataType::Float2:  return 2;
	case ShaderDataType::Float3:  return 3;
	case ShaderDataType::Float4:  return 4;
	case ShaderDataType::Mat3:    return 3 * 3;
	case ShaderDataType::Mat4:    return 4 * 4;
	case ShaderDataType::Int:     return 1;
	case ShaderDataType::Int2:    return 2;
	case ShaderDataType::Int3:    return 3;
	case ShaderDataType::Int4:    return 4;
	case ShaderDataType::Bool:    return 1;
	}

	ASSERT_CORE(false, "Unknown ShaderDataType!")
	return 0;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements): m_Elements(elements)
{
	CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride()
{
	m_Stride = 0;
	uint32 offset = 0;

	for (auto& element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32 size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
		
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
		
	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
		
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLIndexBuffer>(indices, size);
		
	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
