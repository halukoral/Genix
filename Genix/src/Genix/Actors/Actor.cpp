#include "gxpch.h"
#include "Actor.h"

#include "Genix/ECS/EntityManager.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

Actor::Actor()
{
	SEntitySpawnParams spawnParams(1,"Actor");
	Ref<Entity> entity = EntityManager::Get()->SpawnEntity(spawnParams);

	m_TransformComponent = CreateRef<TransformComponent>(entity);
	entity->AddComponent(m_TransformComponent);

	SStaticMeshSpawnParams staticMeshSpawnParams;
	staticMeshSpawnParams.m_entity = entity;
	staticMeshSpawnParams.m_ModelPath = "Assets/Models/Backpack/backpack.obj";
	staticMeshSpawnParams.m_VSPath = "Assets/Shaders/ModelShader.vert";
	staticMeshSpawnParams.m_FSPath = "Assets/Shaders/ModelShader.frag";

	m_StaticMeshComponent = CreateRef<StaticMeshComponent>(staticMeshSpawnParams);
	entity->AddComponent(m_StaticMeshComponent);
}

Actor::~Actor()
{
}

void Actor::SetPosition(const glm::vec3 position) const
{
	m_TransformComponent->SetPosition(position);
}

void Actor::SetRotation(const glm::vec3 rotation) const
{
	m_TransformComponent->SetRotation(rotation);
}

void Actor::SetScale(const glm::vec3 scale) const
{
	m_TransformComponent->SetScale(scale);
}
