#pragma once
#include "Genix/Common/PrimitiveTypes.h"

struct SEntitySpawnParams;
class Entity;
class TimeStep;

class EntityManager
{
public:
	~EntityManager() = default;

	void Update(TimeStep deltaTime);
	void Render();
	void Clear();

	Ref<Entity> SpawnEntity(SEntitySpawnParams& params);

	Ref<Entity> GetEntity(uint32 id) const;

	Ref<Entity> FindEntityByName(const std::string& entityName) const;

	void RemoveEntity(uint32 entityId);

	size_t GetNumEntities() const { return m_Entities.size(); }

	static Ref<EntityManager> Get() { return s_Instance; }
	
private:
	static Ref<EntityManager> s_Instance;
	
	std::vector<Ref<Entity>> m_Entities;
};
