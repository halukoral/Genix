#pragma once

#include "Genix/Actors/Actor.h"

enum class CameraType;
class CameraComponent;
class EditorCameraControllerComponent;
class Event;
class TransformComponent;

class EditorCameraActor : public Actor
{
protected:
	EditorCameraActor();

public:
	EditorCameraActor(EditorCameraActor &other) = delete;
	void operator=(const EditorCameraActor &) = delete;

	void OnUpdate(TimeStep deltaTime) const;
	void OnEvent(Event& e) const;
	void OnResize(uint32 width, uint32 height) const;

	void SetPosition(glm::vec3 position) const;

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(CameraType cameraType) const;
	
	static EditorCameraActor* Get();
	
private:
	static EditorCameraActor* s_Instance;

	Ref<CameraComponent> m_CameraComponent;
	Ref<EditorCameraControllerComponent> m_CameraControllerComponent;
	Ref<TransformComponent> m_TransformComponent;
};
