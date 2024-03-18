#include "gxpch.h"
#include "Camera.h"

#include "Genix/Input/Input.h"
#include "Genix/Input/KeyCodes.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera()
{
	UpdateCameraVectors();
}

Camera::Camera(glm::vec3 position) : m_Position(position)
{
	UpdateCameraVectors();
}

void Camera::OnUpdate(TimeStep ts)
{
	if (Input::IsKeyPressed(GX_KEY_A))
	{
		OnMove(CameraMovement::Left, ts);
	}
	else if (Input::IsKeyPressed(GX_KEY_D))
	{
		OnMove(CameraMovement::Right, ts);
	}

	if (Input::IsKeyPressed(GX_KEY_W))
	{
		OnMove(CameraMovement::Forward, ts);
	}
	else if (Input::IsKeyPressed(GX_KEY_S))
	{
		OnMove(CameraMovement::Backward, ts);
	}
}

void Camera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(GX_BIND(Camera::OnMouseMoved));
	dispatcher.Dispatch<MouseScrolledEvent>(GX_BIND(Camera::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(GX_BIND(Camera::OnWindowResized));
}

void Camera::OnMove(const CameraMovement direction, const TimeStep ts)
{
	const float velocity = m_MovementSpeed * ts;
	if (direction == CameraMovement::Forward)
	{
		m_Position += m_Front * velocity;
	}
	if (direction == CameraMovement::Backward)
	{
		m_Position -= m_Front * velocity;
	}
	if (direction == CameraMovement::Left)
	{
		m_Position -= m_Right * velocity;
	}
	if (direction == CameraMovement::Right)
	{
		m_Position += m_Right * velocity;
	}
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix(const CameraType cameraType) const
{
	if (cameraType == CameraType::Orthographic)
	{
		return glm::ortho(-1.f, 1.f, -1.f, 1.f, m_zNear, m_zFar);
	}
	return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_zNear, m_zFar);
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix(m_CameraType) * GetViewMatrix();	
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void Camera::SetYaw(const float yaw)
{
	m_Yaw += yaw;
}

void Camera::SetPitch(const float pitch)
{
	m_Pitch += pitch;
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_Pitch > 89.0f)
	{
		m_Pitch = 89.0f;
	}
	if (m_Pitch < -89.0f)
	{
		m_Pitch = -89.0f;
	}
}

bool Camera::OnMouseMoved(MouseMovedEvent& e)
{
	if (m_FirstMouseMove)
	{
		m_LastMouseX = e.GetX();	
		m_LastMouseY = e.GetY();
		m_FirstMouseMove = false;
	}
	
	float Xoffset = e.GetX() - m_LastMouseX;
	float Yoffset = m_LastMouseY - e.GetY(); // reversed since y-coordinates go from bottom to top

	m_LastMouseX = e.GetX();
	m_LastMouseY = e.GetY();
	
	Xoffset *= m_MouseSensitivity;
	Yoffset *= m_MouseSensitivity;

	SetYaw(Xoffset);
	SetPitch(Yoffset);
	UpdateCameraVectors();

	return false;
}

bool Camera::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_Fov -= e.GetYOffset() * 0.25f;
	m_Fov = std::max(m_Fov, 0.25f);
	
	return false;
}

bool Camera::OnWindowResized(WindowResizeEvent& e)
{
	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	return false;
}
