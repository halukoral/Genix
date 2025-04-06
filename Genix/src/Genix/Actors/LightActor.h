#pragma once
#include "Actor.h"
#include "Genix/Common/PrimitiveTypes.h"

class LightComponent;
class StaticMeshComponent;
class TransformComponent;

class LightActor : public Actor
{
public:
	LightActor();
	~LightActor() override;
	LightActor(LightActor& other);
	LightActor(const std::string& name);

	LightActor& operator=(LightActor& other);

	glm::vec3 GetLightColor() const;
	glm::vec3 GetLightPos() const;
	
private:
	inline static uint32 s_LightActorCount = 0;
	inline static const std::string s_Name = "LightActor_";

	Ref<LightComponent> m_LightComponent;
	Ref<TransformComponent> m_TransformComponent;
};
