#include "gxpch.h"
#include "Genix/ECS/Components/TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

TransformComponent::TransformComponent(const STransformSpawnParams& params)
{
	Position = params.Position;
	Rotation = params.Rotation;
	Scale = params.Scale;
}

TransformComponent::TransformComponent(TransformComponent& other) : Component(other)
{
	Position = other.Position;
	Rotation = other.Rotation;
	Scale = other.Scale;

	WorldUp = other.WorldUp;

	Front = other.Front;
	Up = other.Up;
	Right = other.Right;
}

TransformComponent& TransformComponent::operator=(TransformComponent& other)
{
	m_Entity = other.m_Entity;

	Position = other.Position;
	Rotation = other.Rotation;
	Scale = other.Scale;

	WorldUp = other.WorldUp;

	Front = other.Front;
	Up = other.Up;
	Right = other.Right;
	return *this;
}

glm::mat4 TransformComponent::GetTransform() const
{
	const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
	return glm::translate(glm::mat4(1.0f), Position)
		* rotation
		* glm::scale(glm::mat4(1.0f), Scale);
}
