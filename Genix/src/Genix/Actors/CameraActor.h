#pragma once

#include "Genix/Actors/Actor.h"

enum class CameraType;
class CameraComponent;
class CameraControllerComponent;
class Event;
class TransformComponent;

class CameraActor : public Actor
{
public:
	CameraActor();
	~CameraActor() override;
	CameraActor(CameraActor& other);
	CameraActor(const std::string& name);

	CameraActor& operator=(CameraActor& other);
	
	void OnUpdate(TimeStep deltaTime) const;
	void OnEvent(Event& e) const;
	void OnResize(uint32 width, uint32 height) const;

	void SetPosition(glm::vec3 position) const;

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(CameraType cameraType) const;

private:
	inline static uint32 s_CameraActorCount = 0;
	inline static const std::string s_Name = "CameraActor_";

	Ref<CameraComponent> m_CameraComponent;
	Ref<CameraControllerComponent> m_CameraControllerComponent;
	Ref<TransformComponent> m_TransformComponent;
};
