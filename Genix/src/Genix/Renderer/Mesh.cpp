#include "gxpch.h"
#include "Genix/Renderer/Mesh.h"
#include "Genix/Renderer/VertexBuffer.h"
#include "Genix/Renderer/RenderCommand.h"
#include "Genix/Renderer/Shader.h"
#include "Genix/Renderer/Texture.h"

#include <assimp/postprocess.h>

SubMesh::SubMesh(const std::vector<Vertex>& vertices, std::vector<uint32>& indices, const std::vector<Ref<Texture>>& textures)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	SetupMesh();
}

void SubMesh::Draw(const Ref<Shader>& shader)
{
	// bind appropriate textures
	uint8 DiffuseNr  = 1;
	uint8 SpecularNr = 1;
	uint8 NormalNr   = 1;
	uint8 HeightNr   = 1;
	
	for(uint8 i = 0; i < m_Textures.size(); i++)
	{
		RenderCommand::SetActiveTexture(i);
		// retrieve texture number (the N in diffuse_textureN)
		std::string Name;
		// switch (m_Textures[i]->GetTextureType())
		// {
		// case TextureType::Diffuse:
		// 	Name = "texture_diffuse" + std::to_string(DiffuseNr++);
		// 	break;
		// case TextureType::Specular:
		// 	Name = "texture_specular" + std::to_string(SpecularNr++);
		// 	break;
		// case TextureType::Normal:
		// 	Name = "texture_normal" + std::to_string(NormalNr++);
		// 	break;
		// case TextureType::Height:
		// 	Name = "texture_height" + std::to_string(HeightNr++);
		// 	break;
		// }

		// now set the sampler to the correct texture unit
		shader->GLSetUniform_Int(Name,i);
		// and finally bind the texture
		m_Textures[i]->Bind(i);
	}

	m_VertexArray->Bind();
	RenderCommand::DrawIndexed(m_VertexArray);

	// always good practice to set everything back to defaults once configured.
	RenderCommand::SetActiveTexture(0);
}

void SubMesh::SetupMesh()
{
	// VAO
	m_VertexArray = VertexArray::Create();

	// VBO
	m_VertexBuffer = VertexBuffer::Create(m_Vertices);
	m_VertexBuffer->SetLayout(
	{
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float3, "a_Tangent" },
		{ ShaderDataType::Float3, "a_Bitangent" },
		{ ShaderDataType::Int4, "a_BoneId" },
		{ ShaderDataType::Float4, "a_Weight" },
		{ ShaderDataType::Int, "a_EntityID" }
	});	
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// EBO
	m_IndexBuffer = IndexBuffer::Create(m_Indices);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	
	m_VertexArray->Unbind();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Mesh::Mesh(std::string const& path, int32_t id, bool gamma) : m_EntityId(id), m_GammaCorrection(gamma)
{
	LoadMesh(path);
}

void Mesh::Init(std::string const& path, int32_t id, bool gamma)
{
	m_EntityId = id;
	m_GammaCorrection = gamma;
	LoadMesh(path);
}

void Mesh::Draw(const Ref<Shader>& shader) const
{
	for (const auto& mesh : m_Meshes)
	{
		mesh->Draw(shader);
	}
}

void Mesh::LoadMesh(std::string const& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		LOG_CORE_ERROR("ERROR:: ASSIMP:: {0} ", importer.GetErrorString());
		return;
	}
	// retrieve the directory path of the filepath
	m_Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void Mesh::ProcessNode(const aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// after we've processed all the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Ref<SubMesh> Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<uint32> indices;
	std::vector<Vertex> vertices;
	std::vector<Ref<Texture>> textures;

	// walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// we declare a placeholder vector since assimp uses its own vector class that doesn't directly
		// convert to glm's vec3 class, so we transfer the data to this placeholder glm::vec3 first.
		glm::vec3 vector;

		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// texture coordinates
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates, so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;

			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertex.EntityID = m_EntityId;
		vertices.push_back(vertex);
	}
	
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);        
		}
	}
	
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

	// return a mesh object created from the extracted mesh data
	return CreateRef<SubMesh>(vertices,indices,textures);
}