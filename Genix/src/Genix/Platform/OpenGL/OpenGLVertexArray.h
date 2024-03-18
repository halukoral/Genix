#pragma once
#include "Genix/Renderer/VertexArray.h"

class GENIX_API OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	~OpenGLVertexArray() override;

	void Bind() const override;
	void Unbind() const override;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	
private:
	uint32 m_Id;
	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
	Ref<IndexBuffer> m_IndexBuffer;
};
