#pragma once
#include "Genix/Common/PrimitiveTypes.h"
#include "Genix/ECS/Component.h"
#include "Genix/Renderer/Mesh.h"

struct SStaticMeshSpawnParams
{
	std::string Name;
	std::string ModelPath;
	std::string VertexShaderPath;
	std::string FragmentShaderPath;
	uint32		IdForMousePick;
	bool Gamma = false; 
};

class StaticMeshComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(StaticMesh)

	StaticMeshComponent() = default;
	~StaticMeshComponent() override;
	StaticMeshComponent(const SStaticMeshSpawnParams& params);
	StaticMeshComponent(StaticMeshComponent& other);

	void Render();
	
	void SetHide(bool hide) { m_Hide = hide; } 
	bool IsHidden() const { return m_Hide; }

	Ref<Mesh> GetMesh() { return m_StaticMesh; }

	std::string GetModelPath() const { return m_ModelPath; }
	std::string GetVertexShaderPath() const { return m_VertexShaderPath; }
	std::string GetFragmentShaderPath() const { return m_FragmentShaderPath; }

private:
	bool m_Hide = false;
	Ref<Mesh> m_StaticMesh;
	Ref<Shader> m_Shader;
	
	std::string m_ModelPath;
	std::string m_VertexShaderPath;
	std::string m_FragmentShaderPath;
};
