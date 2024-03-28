#pragma once
#include "Genix/ECS/Entity.h"
#include "Genix/Common/TimeStep.h"

enum class ComponentType
{
	None,
	Transform,
	StaticMesh
};

class Entity;

class EntityComponent
{
public:
	EntityComponent() = default;
	EntityComponent(const Ref<Entity>& entity);
	virtual ~EntityComponent() = default;
	
	virtual void Update(TimeStep deltaTime) {}
	virtual void Destroy() {}

	ComponentType GetType() const { return m_Type; }
	void SetEntity(const Ref<Entity>& entity) { m_Entity = entity; }
	
protected:
	ComponentType m_Type = ComponentType::None;
	Ref<Entity> m_Entity = nullptr;
};
