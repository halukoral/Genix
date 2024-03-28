#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class ComponentType;

class TimeStep;
class EntityComponent;

struct SEntitySpawnParams
{
	SEntitySpawnParams(uint32 id, std::string name) : m_Id(id), m_Name(name) {}
	uint32 m_Id;
	std::string m_Name;
};

class Entity
{
public:
	using Components = std::vector<Ref<EntityComponent>>;
	
	Entity() = default;
	virtual ~Entity() = default;

	// Called by entity system to complete initialization of the entity.
	Entity(SEntitySpawnParams& params);

	// Called by EntitySystem before entity is destroyed.
	void ShutDown();

	void Update(TimeStep deltaTime) const;
	void Destroy();

	// Retrieves the entity transformation matrix in the world space.
	glm::mat4 GetTransform() const;

	uint32 GetId() const { return m_Id; }
	std::string GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	//! Different from hide in that the entity is not updated
	void SetInvisible(const bool invisible) { m_Invisible = invisible; }
	bool IsInvisible() const { return m_Invisible; }

	virtual bool AddComponent(const Ref<EntityComponent>& component);
	virtual void RemoveComponent(const Ref<EntityComponent>& component);

	bool IsComponentExist(ComponentType type) const;
	Ref<EntityComponent> GetComponent(ComponentType type) const;
	virtual Components& GetComponents() { return m_Components; }
	size_t GetComponentsCount() const { return m_Components.size(); }

private:
	uint32 m_Id;
	std::string m_Name;
	bool m_Invisible = false;
	
	Components m_Components;
};
