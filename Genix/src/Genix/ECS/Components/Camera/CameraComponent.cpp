#include "gxpch.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Entity.h"

CameraComponent::CameraComponent(CameraComponent& other) : Component(other)
{
	m_Fov = other.m_Fov;
	m_AspectRatio = other.m_AspectRatio;

	m_zNear = other.m_zNear;
	m_zFar  = other.m_zFar;

	m_zNear_Ortho = other.m_zNear_Ortho;
	m_zFar_Ortho  = other.m_zFar_Ortho;

	m_OrthoSize = other.m_OrthoSize;
	m_Yaw = other.m_Yaw;
	m_Pitch = other.m_Pitch;
	m_CameraType = other.m_CameraType;
}

CameraComponent& CameraComponent::operator=(CameraComponent& other)
{
	m_Entity = other.m_Entity;
	
	m_Fov = other.m_Fov;
	m_AspectRatio = other.m_AspectRatio;

	m_zNear = other.m_zNear;
	m_zFar  = other.m_zFar;

	m_zNear_Ortho = other.m_zNear_Ortho;
	m_zFar_Ortho  = other.m_zFar_Ortho;

	m_OrthoSize = other.m_OrthoSize;
	m_Yaw = other.m_Yaw;
	m_Pitch = other.m_Pitch;
	m_CameraType = other.m_CameraType;
	return *this;
}

void CameraComponent::UpdateCameraVectors() const
{
	if (const auto& cmp = GetEntity()->GetComponent<TransformComponent>())
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		cmp->Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		cmp->Right = glm::normalize(glm::cross(cmp->Front, cmp->WorldUp));  
		cmp->Up = glm::normalize(glm::cross(cmp->Right, cmp->Front));
	}
}

glm::mat4 CameraComponent::GetViewMatrix() const
{
	if (const auto& cmp = GetEntity()->GetComponent<TransformComponent>())
	{
		return glm::lookAt(cmp->Position, cmp->Position + cmp->Front, cmp->Up);
	}
	return {};
}

glm::mat4 CameraComponent::GetProjectionMatrix(const CameraType cameraType) const
{
	if (cameraType == CameraType::Orthographic)
	{
		return glm::ortho(-m_OrthoSize, m_OrthoSize, -m_OrthoSize, m_OrthoSize, m_zNear_Ortho, m_zFar_Ortho);
	}
	return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_zNear, m_zFar);
}

glm::mat4 CameraComponent::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix(m_CameraType) * GetViewMatrix();	
}

void CameraComponent::SetFOV(const float value)
{
	m_Fov += value;
	m_Fov = std::max(m_Fov, 0.25f);
}

void CameraComponent::SetYaw(const float yaw)
{
	m_Yaw += yaw;
}

void CameraComponent::SetPitch(const float pitch)
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
