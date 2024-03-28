#pragma once
#include "Genix/ECS/EntityComponent.h"
#include "Genix/Renderer/Model.h"

struct SStaticMeshSpawnParams
{
	Ref<Entity> m_entity;
	std::string m_ModelPath;
	std::string m_VSPath;
	std::string m_FSPath;
	bool m_gamma = false; 
};

class StaticMeshComponent : public EntityComponent
{
public:
	StaticMeshComponent() = delete;
	StaticMeshComponent(const SStaticMeshSpawnParams& params);

	void Update(TimeStep deltaTime) override;
	
	void SetHide(bool hide) { m_Hide = hide; } 
	bool IsHidden() const { return m_Hide; }

	Ref<Model> GetMesh() { return m_StaticMesh; }
	
private:
	bool m_Hide = false;
	Ref<Model> m_StaticMesh;
	Ref<Shader> m_Shader;
};
