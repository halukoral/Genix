#include "gxpch.h"
#include "Genix/ECS/Entity.h"
#include "Genix/Utils/PlatformUtils.h"

Entity::Entity(const Entity& other)
{
	m_Id = other.m_Id;
	m_Name = other.m_Name;
	m_IdForMousePick = other.m_IdForMousePick;
}

Entity::Entity(const std::string& name)
{
	m_Id = GUUID();
	m_Name = name;
	m_IdForMousePick = Rnd::GetRandValue();
}

Entity::Entity(const GUUID& uuid, const std::string& name)
{
	m_Id = uuid;
	m_Name = name;
	m_IdForMousePick = Rnd::GetRandValue();
}

void Entity::ShutDown()
{
	for (auto cmp : m_Components)
	{
		cmp.reset();
	}
	m_Components.clear();
	m_ComponentTypeMap.clear();
	LOG_CORE_INFO("Entity {0} {1} destroyed!", m_Id, m_Name);
}
