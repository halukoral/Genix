#include "gxpch.h"
#include "EntityManager.h"

#include "Entity.h"
#include "Genix/Common/TimeStep.h"

Ref<EntityManager> EntityManager::s_Instance = CreateRef<EntityManager>();

void EntityManager::Update(TimeStep deltaTime)
{
	for (auto entity : m_Entities)
	{
		entity->Update(deltaTime);
	}
}

void EntityManager::Render()
{
	for (auto entity : m_Entities)
	{
		entity->Render();
	}
}

void EntityManager::Clear()
{
	for (auto entity : m_Entities)
	{
		entity->ShutDown();
	}
}

Ref<Entity> EntityManager::SpawnEntity(SEntitySpawnParams& params)
{
	Ref<Entity> entity = CreateRef<Entity>(params);
	m_Entities.push_back(entity);
	return entity;
}

Ref<Entity> EntityManager::GetEntity(const uint32 id) const
{
	for (auto entity : m_Entities)
	{
		if (entity->GetId() == id)
		{
			return entity;
		}
	}
	return nullptr;
}

Ref<Entity> EntityManager::FindEntityByName(const std::string& entityName) const
{
	for (auto entity : m_Entities)
	{
		if (entity->GetName() == entityName)
		{
			return entity;
		}
	}
	return nullptr;
}

void EntityManager::RemoveEntity(uint32 entityId)
{
	const auto it = std::find_if(
		m_Entities.begin(),
		m_Entities.end(),
		[entityId](const Ref<Entity>& entity) -> bool { return entity->GetId() == entityId; });

	if (it != m_Entities.end())
	{
		m_Entities.erase(it);
	}
}
