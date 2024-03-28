#include "gxpch.h"
#include "Genix/ECS/Entity.h"
#include "Genix/ECS/EntityComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/Common/TimeStep.h"

Entity::Entity(SEntitySpawnParams& params)
{
	m_Name = params.m_Name;
	m_Id = params.m_Id;
}

void Entity::ShutDown()
{
	Destroy();
}

void Entity::Update(TimeStep deltaTime) const
{
	for(const auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void Entity::Destroy()
{
	for(const auto& component : m_Components)
	{
		component->Destroy();
	}
	m_Components.clear();
}

glm::mat4 Entity::GetTransform() const
{
	if (IsComponentExist(ComponentType::Transform))
	{
		Ref<EntityComponent> cmp = GetComponent(ComponentType::Transform);
		return std::dynamic_pointer_cast<TransformComponent>(cmp)->GetTransform();
	}
	LOG_CORE_WARN("Entity does not have transform!");
	return {};
}

bool Entity::AddComponent(const Ref<EntityComponent>& component)
{
	if (m_Components.empty() || IsComponentExist(component->GetType()) == false)
	{
		m_Components.push_back(component);
		return true;
	}
	ASSERT_CORE(false, "Entity::AddComponent couldn't add component")
	return false;
}

void Entity::RemoveComponent(const Ref<EntityComponent>& component)
{
	const auto it = std::find_if(
	m_Components.begin(),
	m_Components.end(),
	[component](const Ref<EntityComponent>& entityComponent) -> bool { return entityComponent->GetType() == component->GetType(); });

	if (it != m_Components.end())
	{
		m_Components.erase(it);
	}
}

bool Entity::IsComponentExist(const ComponentType type) const
{
	for(const auto& component : m_Components)
	{
		if (component->GetType() == type)
		{
			return true;
		}
	}
	return false;
}

Ref<EntityComponent> Entity::GetComponent(const ComponentType type) const
{
	for(auto component : m_Components)
	{
		if (component->GetType() == type)
		{
			return component;
		}
	}
	return nullptr;
}