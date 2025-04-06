#pragma once
#include "Entity.h"
#include "Genix/Common/PrimitiveTypes.h"
#include "Genix/ECS/Systems/RenderingManager.h"

class UUID;
class Entity;
class TimeStep;

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager() = default;

	template<typename T, typename... Args>
	Ref<T> SpawnEntity(Args&&... args)
	{
		Ref<T> entity = CreateRef<T>(std::forward<Args>(args)...);
		entity->SetSelf(entity);
		m_Entities.push_back(entity);
		return std::static_pointer_cast<T>(entity);
	}
	
	template<typename T>
	Ref<Entity> GetFirstEntityByComponent()
	{
		for (int i = 0; i < (int) m_Entities.size(); i++)
		{
			if (auto cmp = m_Entities[i]->GetComponent<T>())
			{
				if (cmp->GetComponentType() == T::GetStaticType())
				{
					return m_Entities[i];
				}
			}
		}
		LOG_CORE_WARN("The scene has no entities with requested component.");
		return nullptr;
	}

	Ref<Entity> GetEntity(UUID id) const;
	Ref<Entity> FindEntityByName(const std::string& entityName) const;

	void Clear() const;
	void RemoveEntity(UUID entityId);
	size_t GetEntityCount() const { return m_Entities.size(); }

	static Ref<EntityManager> Get() { return s_Instance; }

	friend RenderingManager;

protected:
	std::vector<Ref<Entity>> m_Entities;

private:
	static Ref<EntityManager> s_Instance;
	
};
