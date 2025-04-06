#pragma once
#include "Genix/Common/PrimitiveTypes.h"
#include "Genix/ECS/Component.h"

enum class CameraMovement;
class CameraComponent;
class Event;
class MouseScrolledEvent;
class MouseMovedEvent;
class TimeStep;
class WindowResizeEvent;

class CameraControllerComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(CameraController)
	
	CameraControllerComponent() = default;
	CameraControllerComponent(CameraControllerComponent& other);
	CameraControllerComponent& operator=(CameraControllerComponent& other);

	virtual void OnUpdate(TimeStep deltaTime);
	virtual void OnEvent(Event& e);
	virtual void OnMove(CameraMovement direction, TimeStep deltaTime) const;
	
	void OnResize(uint32 width, uint32 height);
	void SetPosition(CameraMovement direction, float velocity) const;

	bool GetMouseFirstMove() const { return m_FirstMouseMove; }
	void SetMouseFirstMove(float value) { m_FirstMouseMove = value; }

	std::pair<float,float> GetMousePos() const { return {m_LastMouseX, m_LastMouseY}; }
	void SetMousePos(std::pair<float,float> value) { m_LastMouseX = value.first; m_LastMouseY = value.second; }

	float GetMovementSpeed() const { return m_MovementSpeed; }
	void SetMovementSpeed(float value) { m_MovementSpeed = value; }

	float GetPanSpeed() const { return m_PanSpeed; }
	void SetPanSpeed(float value) { m_PanSpeed = value; }

	float GetMouseSensitivity() const { return m_MouseSensitivity; }
	void SetMouseSensitivity(float value) { m_MouseSensitivity = value; }
	
	glm::vec2 GetViewportSize() const { return m_ViewportSize; }
	void SetViewportSize(glm::vec2 value) { m_ViewportSize = value; }
	
protected:
	virtual bool OnMouseMoved(MouseMovedEvent& e);
	bool OnMousePanned(MouseMovedEvent& e);
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);
	
protected:
	bool  m_FirstMouseMove = true;
	float m_LastMouseX;
	float m_LastMouseY;
	float m_MovementSpeed = 5.f;
	float m_PanSpeed = 0.025f;
	float m_MouseSensitivity = 0.1f;

	glm::vec2 m_ViewportSize;
};
