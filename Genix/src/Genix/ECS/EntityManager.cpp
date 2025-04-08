#include "gxpch.h"
#include "Genix/ECS/EntityManager.h"
#include "Genix/ECS/Entity.h"

Ref<EntityManager> EntityManager::s_Instance = CreateRef<EntityManager>();

void EntityManager::Clear() const
{
	for (const auto& entity : m_Entities)
	{
		entity->ShutDown();
	}
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

Ref<Entity> EntityManager::GetEntity(const GUUID id) const
{
	for (auto entity : m_Entities)
	{
		if (entity->GetUUID() == id)
		{
			return entity;
		}
	}
	return nullptr;
}

void EntityManager::RemoveEntity(GUUID entityId)
{
	const auto it = std::find_if(
		m_Entities.begin(),
		m_Entities.end(),
		[entityId](const Ref<Entity>& entity) -> bool { return entity->GetUUID() == entityId; });

	if (it != m_Entities.end())
	{
		m_Entities.erase(it);
	}
}
