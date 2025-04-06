#pragma once
#include "Genix/Renderer/VertexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

	const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	static Ref<VertexArray> Create();
private:
	uint32 m_Id;
	uint32 m_VertexBufferIndex = 0;
	
	Ref<IndexBuffer> m_IndexBuffer;
	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
};
