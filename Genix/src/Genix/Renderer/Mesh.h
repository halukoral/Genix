#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Genix/Common/PrimitiveTypes.h"

enum class TextureType;
struct Vertex;

class Shader;
class Texture;

class IndexBuffer;
class VertexArray;
class VertexBuffer;

class SubMesh
{
public:
	SubMesh() = default;
	SubMesh(const std::vector<Vertex>& vertices, std::vector<uint32>& indices, const std::vector<Ref<Texture>>& textures);
	virtual ~SubMesh() = default;

	void Draw(const Ref<Shader>& shader);
	
protected:
	void SetupMesh();

	// mesh Data
	std::vector<Vertex> m_Vertices;
	std::vector<uint32> m_Indices;
	std::vector<Ref<Texture>> m_Textures;

	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_VertexBuffer;
	Ref<IndexBuffer>  m_IndexBuffer;
};

class Mesh 
{
public:
	Mesh() = default;
	Mesh(std::string const &path, int32 id = -1, bool gamma = false);
	
	void Init(std::string const &path, int32 id = -1, bool gamma = false);
	void Draw(const Ref<Shader>& shader) const;

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void LoadMesh(std::string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void ProcessNode(const aiNode *node, const aiScene *scene);

	Ref<SubMesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);

private:
	int32 m_EntityId;
	bool m_GammaCorrection;
	std::string m_Directory;
	std::vector<Ref<SubMesh>> m_Meshes;
};