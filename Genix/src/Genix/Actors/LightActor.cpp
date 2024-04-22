#include "gxpch.h"
#include "Genix/Actors/LightActor.h"
#include "Genix/ECS/Components/LightComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

LightActor::LightActor() : Actor(s_Name + std::to_string(s_LightActorCount++))
{
	m_LightComponent = m_Entity->AddComponent<LightComponent>();
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();
	m_TransformComponent->Position = {3.f, 3.f, 3.f};
}

LightActor::LightActor(const std::string& name) : Actor(name)
{
	m_LightComponent = m_Entity->AddComponent<LightComponent>();
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();
}

LightActor::~LightActor()
{
	m_LightComponent.reset();
	m_TransformComponent.reset();
}

LightActor::LightActor(LightActor& other)
{
	*m_LightComponent = *other.m_LightComponent;
	*m_TransformComponent = *other.m_TransformComponent;
}

LightActor& LightActor::operator=(LightActor& other)
{
	*m_LightComponent = *other.m_LightComponent;
	*m_TransformComponent = *other.m_TransformComponent;
	return *this;
}

glm::vec3 LightActor::GetLightColor() const
{
	return m_LightComponent->GetLightColor();
}

glm::vec3 LightActor::GetLightPos() const
{
	return m_TransformComponent->Position;
}
