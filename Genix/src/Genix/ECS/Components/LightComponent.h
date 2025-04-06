#pragma once
#include "Genix/ECS/Component.h"


class TransformComponent;

class LightComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Light)

	LightComponent() = default;
	LightComponent(LightComponent& other);
	LightComponent& operator=(LightComponent& other);

	glm::vec3& GetLightColor() { return m_LightColor; }
	void SetLightColor(glm::vec3 value) { m_LightColor = value; }
	glm::vec3 GetLightPos() const;
	
private:
	glm::vec3 m_LightColor = glm::vec3(300.0f, 300.0f, 300.0f);
};
