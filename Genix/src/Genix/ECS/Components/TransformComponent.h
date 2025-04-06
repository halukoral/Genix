#pragma once
#include "Genix/ECS/Component.h"

struct STransformSpawnParams
{
	STransformSpawnParams() = default;

	STransformSpawnParams(const glm::vec3 p, const glm::vec3 r, const glm::vec3 s)
		: Position(p), Rotation(r), Scale(s) {}

	glm::vec3 Position { 0.f };
	glm::vec3 Rotation { 0.f };
	glm::vec3 Scale { 1.f };
};

class TransformComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Transform)
	
	TransformComponent() = default;
	TransformComponent(const STransformSpawnParams& params);
	TransformComponent(TransformComponent& other);
	TransformComponent& operator=(TransformComponent& other);
	
	glm::mat4 GetTransform() const;

	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::vec3 Position { 0.f };
	glm::vec3 Rotation { 0.f };
	glm::vec3 Scale { 1.f };
	
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
};
