#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

enum class TextureType;
class Mesh;
class Shader;
class Texture;

class Model 
{
public:
	// constructor, expects a filepath to a 3D model.
	Model(std::string const &path, bool gamma = false);

	void Init(std::string const &path, bool gamma = false);
	
	// draws the model, and thus all its meshes
	void Draw(const Ref<Shader>& shader);

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void LoadModel(std::string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void ProcessNode(const aiNode *node, const aiScene *scene);

	Ref<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const TextureType texturetype);

	// model data 
	bool m_GammaCorrection;
	std::string m_Directory;
	std::vector<Ref<Mesh>>    m_Meshes;
	std::vector<Ref<Texture>> m_TexturesLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
};