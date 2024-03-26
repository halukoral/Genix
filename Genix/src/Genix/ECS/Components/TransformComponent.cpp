#include "gxpch.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Entity.h"

TransformComponent::TransformComponent(const Ref<Entity>& entity)
	: EntityComponent(entity)
{
	m_Type = ComponentType::Transform;
}

glm::mat4 TransformComponent::GetTransform() const
{
	const glm::mat4 rotation =
		glm::rotate(glm::mat4(1.0f), m_Rotation.x, { 1, 0, 0 })
		* glm::rotate(glm::mat4(1.0f), m_Rotation.y, { 0, 1, 0 })
		* glm::rotate(glm::mat4(1.0f), m_Rotation.z, { 0, 0, 1 });

	return glm::translate(glm::mat4(1.0f), m_Position)
		* rotation
		* glm::scale(glm::mat4(1.0f), m_Scale);
}

void TransformComponent::SetPosition(const glm::vec3 position)
{
	if (m_Position == position)
		return;

	m_Position = position;
}

void TransformComponent::SetRotation(const glm::vec3 rotation)
{
	if (m_Rotation == rotation)
		return;

	m_Rotation = rotation;
}

void TransformComponent::SetScale(const glm::vec3 scale)
{
	if (m_Scale == scale)
		return;

	m_Scale = scale;
}

void TransformComponent::SetPosRotScale(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale)
{
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);
}
