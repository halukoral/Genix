#include "gxpch.h"
#include "Model.h"

#include "Buffer.h"
#include "Mesh.h"

#include <assimp/postprocess.h>


Model::Model(std::string const& path, bool gamma) : m_GammaCorrection(gamma)
{
	LoadModel(path);
}

void Model::Draw(const Ref<Shader>& shader)
{
	for (const auto& Mesh : m_Meshes)
	{
		Mesh->Draw(shader);
	}
}

void Model::LoadModel(std::string const& path)
{
	// read file via ASSIMP
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if(!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) // if is Not Zero
	{
		LOG_CORE_ERROR("ERROR:: ASSIMP:: {0} ", Importer.GetErrorString());
		return;
	}
	// retrieve the directory path of the filepath
	m_Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode(Scene->mRootNode, Scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* Mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(Mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Ref<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<uint32> indices;
	std::vector<VertexData> vertices;
	std::vector<Ref<Texture>> textures;

	// walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;
		// we declare a placeholder vector since assimp uses its own vector class that doesn't directly
		// convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		glm::vec3 vector;

		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_Position = vector;

		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_Normal = vector;
		}

		// texture coordinates
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_TexCoords = vec;

			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.m_Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.m_Bitangent = vector;
		}
		else
		{
			vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
		}

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

	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Ref<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. specular maps
	std::vector<Ref<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<Ref<Texture>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Normal);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. height maps
	std::vector<Ref<Texture>> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Height);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh::Create(vertices,indices,textures);
}

std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const TextureType texturetype)
{
	std::vector<Ref<Texture>> Textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool Skip = false;
		for (const auto& texture : m_TexturesLoaded)
		{
			if(std::strcmp(texture->GetPath().data(), str.C_Str()) == 0)
			{
				Textures.push_back(texture);
				Skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		
		if(!Skip)
		{
			// if texture hasn't been loaded already, load it
			Ref<Texture> texture = Texture::Create(m_Directory + '/' +str.C_Str());
			texture->SetTextureType(texturetype);
			texture->SetPath(str.C_Str());
			Textures.push_back(texture);

			// store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			m_TexturesLoaded.push_back(texture);
		}
	}
	return Textures;
}
