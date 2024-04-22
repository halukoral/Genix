#include "gxpch.h"
#include "Genix/ECS/Components/LightComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Entity.h"

LightComponent::LightComponent(LightComponent& other) : Component(other)
{
	m_LightColor = other.m_LightColor;
}

LightComponent& LightComponent::operator=(LightComponent& other)
{
	m_Entity = other.m_Entity;

	m_LightColor = other.m_LightColor;
	return *this;
}

glm::vec3 LightComponent::GetLightPos() const
{
	if (const auto& cmp = GetEntity()->GetComponent<TransformComponent>())
	{
		return cmp->Position;
	}
	return {};
}
