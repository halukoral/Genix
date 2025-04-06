#pragma once
#include "Genix/Core/Core.h"
#include "Genix/ECS/Entity.h"
#include "Genix/ECS/EntityManager.h"

class Actor
{
public:
	Actor()
	{
		m_Entity = EntityManager::Get()->SpawnEntity<Entity>(s_Name + std::to_string(s_ActorCount++));
	}
	
	Actor(const std::string& name)
	{
		m_Entity = EntityManager::Get()->SpawnEntity<Entity>(name);
	}

	Actor(const UUID& uuid, const std::string& name)
	{
		m_Entity = EntityManager::Get()->SpawnEntity<Entity>(uuid, name);
	}

	virtual ~Actor()
	{
		m_Entity->ShutDown();
		m_Entity.reset();
	}

	Ref<Entity> GetEntity() const { return m_Entity; }

protected:
	
	Ref<Entity> m_Entity;
	
private:
	inline static const std::string s_Name = "Actor_";
	inline static uint32 s_ActorCount = 0;
};
