#pragma once
#include "Genix/ECS/Components/Camera/CameraControllerComponent.h"

class MouseScrolledEvent;
class TransformComponent;
class Event;
class TimeStep;

class EditorCameraControllerComponent : public CameraControllerComponent
{
public:
	COMPONENT_CLASS_TYPE(EditorCameraController)
	
	EditorCameraControllerComponent() = default;
	
	void OnUpdate(TimeStep deltaTime) override;
	void OnEvent(Event& e) override;
	bool OnMouseMoved(MouseMovedEvent& e) override;
};
