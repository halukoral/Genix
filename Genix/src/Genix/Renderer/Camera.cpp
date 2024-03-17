#include "gxpch.h"
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(glm::vec3 InPosition, glm::vec3 InUp, float InYaw, float InPitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = InPosition;
	WorldUp = InUp;
	Yaw = InYaw;
	Pitch = InPitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	return glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.0f, 1.0f) * GetViewMatrix();	
	//return glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f) * GetViewMatrix();
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 LocalFront;
	LocalFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	LocalFront.y = sin(glm::radians(Pitch));
	LocalFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(LocalFront);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}