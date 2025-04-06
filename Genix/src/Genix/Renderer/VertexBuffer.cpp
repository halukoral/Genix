#include "gxpch.h"
#include "Genix/Renderer/VertexBuffer.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/RendererAPI.h"

#include <glad/glad.h>

uint32 BufferElement::GetComponentCount() const
{
	switch (Type)
	{
	case ShaderDataType::Float:   return 1;
	case ShaderDataType::Float2:  return 2;
	case ShaderDataType::Float3:  return 3;
	case ShaderDataType::Float4:  return 4;
	case ShaderDataType::Mat3:    return 3; // 3* float3
	case ShaderDataType::Mat4:    return 4; // 4* float4
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
	size_t  offset = 0;

	for (auto& element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32 size)
{
	return CreateRef<VertexBuffer>(vertices, size);
}

Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex> vertices)
{
	return CreateRef<VertexBuffer>(vertices);
}

Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 size)
{
	return CreateRef<IndexBuffer>(indices, size);
}

Ref<IndexBuffer> IndexBuffer::Create(std::vector<uint32> indices)
{
	return CreateRef<IndexBuffer>(indices);
}

// ------------------------------------Vertex Buffer-------------------------------
// --------------------------------------------------------------------------------

VertexBuffer::VertexBuffer(const float* vertices, const uint32 size)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	GX_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::Bind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ------------------------------------Index Buffer--------------------------------
// --------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(uint32* indices, uint32 count) : m_Count(count)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32), indices, GL_STATIC_DRAW);	
}

IndexBuffer::IndexBuffer(const std::vector<uint32>& indices) : m_Count((uint32)indices.size())
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	GX_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_Id);
}

void IndexBuffer::Bind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBuffer::Unbind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}