#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class Shader;
class Texture;

class IndexBuffer;
class VertexArray;
class VertexBuffer;

struct VertexData;

class Mesh {
public:
	Mesh() = default;
	virtual ~Mesh() = default;

	virtual void Draw(const Ref<Shader>& shader) = 0;

	static Ref<Mesh> Create(std::vector<VertexData> vertices, std::vector<uint32> indices, std::vector<Ref<Texture>> textures);

protected:
	virtual void SetupMesh() = 0;

	// mesh Data
	std::vector<VertexData> m_Vertices;
	std::vector<uint32> m_Indices;
	std::vector<Ref<Texture>> m_Textures;

};
