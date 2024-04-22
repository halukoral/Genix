#include "gxpch.h"
#include "Genix/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

// ------------------------------------Vertex Buffer-------------------------------
// --------------------------------------------------------------------------------

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32 size)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex>& vertices)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	GX_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_Id);
}

void OpenGLVertexBuffer::Bind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void OpenGLVertexBuffer::Unbind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ------------------------------------Index Buffer--------------------------------
// --------------------------------------------------------------------------------

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count) : m_Count(count)
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32), indices, GL_STATIC_DRAW);	
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32>& indices) : m_Count((uint32)indices.size())
{
	GX_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_Id);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32), indices.data(), GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	GX_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_Id);
}

void OpenGLIndexBuffer::Bind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void OpenGLIndexBuffer::Unbind() const
{
	GX_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
