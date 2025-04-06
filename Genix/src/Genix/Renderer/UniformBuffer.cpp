#include "gxpch.h"
#include "Genix/Renderer/UniformBuffer.h"
#include "Genix/Renderer/Renderer.h"

#include <glad/glad.h>

Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding)
{
	return CreateRef<UniformBuffer>(size, binding);

}

UniformBuffer::UniformBuffer(uint32 size, uint32 binding)
{
	glCreateBuffers(1, &m_Id);
	glNamedBufferData(m_Id, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_Id);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void UniformBuffer::SetData(const void* data, uint32 size, uint32 offset)
{
	glNamedBufferSubData(m_Id, offset, size, data);
}