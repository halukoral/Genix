#include "gxpch.h"
#include "Genix/Actors/StaticMeshActor.h"

#include "Genix/ECS/Components/PBRMaterialComponent.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

StaticMeshActor::StaticMeshActor() : Actor(s_Name + std::to_string(s_StaticMeshActorCount++))
{
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();
	SStaticMeshSpawnParams staticMeshSpawnParams;
	staticMeshSpawnParams.ModelPath = "Assets/Models/Basic/Sphere.fbx";
	staticMeshSpawnParams.VertexShaderPath = "Assets/Shaders/PBR.vert";
	staticMeshSpawnParams.FragmentShaderPath = "Assets/Shaders/PBR.frag";
	staticMeshSpawnParams.IdForMousePick = m_Entity->GetIdForMousePicking();
	m_StaticMeshComponent = m_Entity->AddComponent<StaticMeshComponent>(staticMeshSpawnParams);
	m_PBR_MaterialComponent = m_Entity->AddComponent<PBRMaterialComponent>();
}

StaticMeshActor::StaticMeshActor(SStaticMeshSpawnParams& params) : Actor(params.Name)
{
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();
	params.IdForMousePick = m_Entity->GetIdForMousePicking();
	m_StaticMeshComponent = m_Entity->AddComponent<StaticMeshComponent>(params);
	m_PBR_MaterialComponent = m_Entity->AddComponent<PBRMaterialComponent>();
}

StaticMeshActor::~StaticMeshActor()
{
	m_TransformComponent.reset();
}
