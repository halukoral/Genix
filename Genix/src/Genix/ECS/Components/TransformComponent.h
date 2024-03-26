#pragma once
#include "Genix/ECS/EntityComponent.h"

class TransformComponent : public EntityComponent
{
public:
	TransformComponent(const Ref<Entity>& entity);
	~TransformComponent() override = default;

	glm::mat4 GetTransform() const;
	
	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetRotation() const { return m_Rotation; }
	glm::vec3 GetScale() const { return m_Scale; }

	void SetPosition(const glm::vec3 position);
	void SetRotation(const glm::vec3 rotation);
	void SetScale(const glm::vec3 scale);
	void SetPosRotScale(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale);

private:
	glm::vec3 m_Position { 0.f };
	glm::vec3 m_Rotation { 0.f };
	glm::vec3 m_Scale { 1.f };
};
