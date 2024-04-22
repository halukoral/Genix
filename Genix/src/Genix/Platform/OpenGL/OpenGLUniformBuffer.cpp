#include "gxpch.h"
#include "Genix/Platform/OpenGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding)
{
	glCreateBuffers(1, &m_Id);
	glNamedBufferData(m_Id, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_Id);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
	glDeleteBuffers(1, &m_Id);
}


void OpenGLUniformBuffer::SetData(const void* data, uint32 size, uint32 offset)
{
	glNamedBufferSubData(m_Id, offset, size, data);
}