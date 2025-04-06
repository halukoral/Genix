#pragma once
#include "Genix/Actors/Actor.h"

class BPMaterialComponent;
class PBRMaterialComponent;
class StaticMeshComponent;
class TransformComponent;

class StaticMeshActor : public Actor
{
public:
	StaticMeshActor();
	StaticMeshActor(SStaticMeshSpawnParams& params);
	~StaticMeshActor() override;
	
private:
	inline static uint32 s_StaticMeshActorCount = 0;
	inline static const std::string s_Name = "StaticMeshActor_";

	Ref<BPMaterialComponent> m_BP_MaterialComponent;
	Ref<PBRMaterialComponent> m_PBR_MaterialComponent;
	
	Ref<StaticMeshComponent> m_StaticMeshComponent;
	Ref<TransformComponent> m_TransformComponent;
};
