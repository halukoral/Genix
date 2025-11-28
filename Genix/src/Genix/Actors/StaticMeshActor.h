#pragma once
#include "Genix/Actors/Actor.h"

class MaterialComponent;
class StaticMeshComponent;

class StaticMeshActor : public Actor
{
public:
	StaticMeshActor();
	StaticMeshActor(SStaticMeshSpawnParams& params);
	~StaticMeshActor() override;
	
private:
	inline static uint32 s_StaticMeshActorCount = 0;
	inline static const std::string s_Name = "StaticMeshActor_";

	Ref<MaterialComponent> m_MaterialComponent;
	Ref<StaticMeshComponent> m_StaticMeshComponent;
};
