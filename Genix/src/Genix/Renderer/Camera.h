#pragma once

#include "Genix/Events/ApplicationEvent.h"
#include "Genix/Events/MouseEvent.h"
#include "Genix/Common/TimeStep.h"

#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement
{
	Forward,
	Backward,
	Left,
	Right
};

enum class CameraType
{
	Perspective,
	Orthographic
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);

	void OnUpdate(TimeStep deltaTime);
	void OnEvent(Event& e);
	void OnResize(float width, float height);

	void OnMove(CameraMovement direction, TimeStep deltaTime);
	
	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(CameraType cameraType) const;
	glm::mat4 GetViewProjectionMatrix() const;

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position);

private:
	void SetYaw(float yaw);
	void SetPitch(float pitch);

	bool OnMouseMoved(MouseMovedEvent& e);
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);
	
	// calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();
	
	// camera Attributes
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	CameraType m_CameraType = CameraType::Perspective;

	bool  m_FirstMouseMove = true;
	float m_LastMouseX;
	float m_LastMouseY;
	
	// camera options
	float m_Fov = 45.0f;
	float m_AspectRatio = 1920.f / 1080.f;
	float m_MovementSpeed = 2.5f;
	float m_MouseSensitivity = 0.1f;

	float m_zNear = 0.1f;
	float m_zFar  = 100.f;
	
	// euler Angles
	float m_Yaw = -90.0f;
	float m_Pitch = 0.f;
};
