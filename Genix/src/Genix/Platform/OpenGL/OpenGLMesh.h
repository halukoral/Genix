#pragma once
#include "Genix/Renderer/Mesh.h"

class OpenGLMesh : public Mesh
{
public:
	OpenGLMesh(const std::vector<VertexData>& vertices, uint32*& indices, const std::vector<Ref<Texture>>& textures);
	void Draw(const Ref<Shader>& shader) override;

protected:
	void SetupMesh() override;

	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_VertexBuffer;
	Ref<IndexBuffer>  m_IndexBuffer;
};
