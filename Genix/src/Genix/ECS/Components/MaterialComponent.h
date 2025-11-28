#pragma once
#include "Genix/ECS/Component.h"

class MaterialComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Material)
	
	MaterialComponent() = default;
	MaterialComponent(MaterialComponent& other) : Component(other)
	{
		Albedo = other.Albedo;
		Roughness = other.Roughness;
		Metallic = other.Metallic;
		EmissionColor = other.EmissionColor;
		EmissionPower = other.EmissionPower;
	}

	MaterialComponent& operator=(MaterialComponent& other)
	{
		m_Entity = other.m_Entity;

		Albedo = other.Albedo;
		Roughness = other.Roughness;
		Metallic = other.Metallic;
		EmissionColor = other.EmissionColor;
		EmissionPower = other.EmissionPower;
		return *this;
	}

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }

	glm::vec3	Albedo{ 1.0f };
	float		Roughness = 1.0f;
	float		Metallic = 0.0f;
	glm::vec3	EmissionColor{ 0.0f };
	float		EmissionPower = 0.0f;
};
