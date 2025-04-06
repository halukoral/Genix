#pragma once
#include "Genix/ECS/Component.h"

class PBRMaterialComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Material_PBR)
	
	PBRMaterialComponent() = default;
	PBRMaterialComponent(PBRMaterialComponent& other) : Component(other)
	{
		Albedo = other.Albedo;
		Roughness = other.Roughness;
		Metallic = other.Metallic;
		EmissionColor = other.EmissionColor;
		EmissionPower = other.EmissionPower;
	}

	PBRMaterialComponent& operator=(PBRMaterialComponent& other)
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
