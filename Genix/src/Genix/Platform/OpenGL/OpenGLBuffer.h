#pragma once
#include "Genix/Renderer/Buffer.h"

class GENIX_API OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32 size);
	~OpenGLVertexBuffer() override;

	void Bind() const override;
	void Unbind() const override;

	const BufferLayout& GetLayout() const override { return m_Layout; }
	void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	
private:
	uint32 m_Id;
	BufferLayout m_Layout;
};

class GENIX_API OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32* indices, uint32 count);
	~OpenGLIndexBuffer() override;

	void Bind() const override;
	void Unbind() const override;

	uint32 GetCount() const override { return m_Count; }
private:
	uint32 m_Id;
	uint32 m_Count;
};
