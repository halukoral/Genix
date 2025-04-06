#include "gxpch.h"
#include "Genix/Renderer/VertexArray.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/RendererAPI.h"

#include <glad/glad.h>

Ref<VertexArray> VertexArray::Create()
{
	return CreateRef<VertexArray>();
}

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    
	case ShaderDataType::Float2:   
	case ShaderDataType::Float3:   
	case ShaderDataType::Float4:   
	case ShaderDataType::Mat3:     
	case ShaderDataType::Mat4:
		return GL_FLOAT;

	case ShaderDataType::Int:      
	case ShaderDataType::Int2:     
	case ShaderDataType::Int3:     
	case ShaderDataType::Int4:
		return GL_INT;

	case ShaderDataType::Bool:
		return GL_BOOL;
	}

	ASSERT_CORE(false, "Unknown ShaderDataType!")
	return 0;
}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	ASSERT_CORE(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

	glBindVertexArray(m_Id);
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		switch (element.Type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			{
				uint8 count = element.GetComponentCount();
				for (uint8 i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
		default:
			ASSERT_CORE(false, "Unknown ShaderDataType!");
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_Id);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}
