#pragma once
#include "Genix/ECS/Component.h"

class CameraControllerComponent;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement
{
	Up,
	Down,
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

class CameraComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Camera)
	friend CameraControllerComponent;

	CameraComponent() = default;
	CameraComponent(CameraComponent& other);
	CameraComponent& operator=(CameraComponent& other);

	void UpdateCameraVectors() const;

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(CameraType cameraType) const;
	glm::mat4 GetViewProjectionMatrix() const;
	
	CameraType GetCameraType() const { return m_CameraType; }
	void SetCameraType(const CameraType type) { m_CameraType = type; }

	float GetFOV() const { return m_Fov; }
	void SetFOV(const float value);

	float GetAspectRatio() const { return m_AspectRatio; }
	void SetAspectRatio(const float value) { m_AspectRatio = value; }

	float GetNearClip() const { return m_zNear; }
	void SetNearClip(const float value) { m_zNear = value; }

	float GetFarClip() const { return m_zFar; }
	void SetFarClip(const float value) { m_zFar = value; }

	float GetOrthoClip() const { return m_OrthoSize; }
	void SetOrthoClip(const float value) { m_OrthoSize = value; }

	float GetNearClipOrtho() const { return m_zNear_Ortho; }
	void SetNearClipOrtho(const float value) { m_zNear_Ortho = value; }

	float GetFarClipOrtho() const { return m_zFar_Ortho; }
	void SetFarClipOrtho(const float value) { m_zFar_Ortho = value; }
	
	float GetYaw() const { return m_Yaw; }
	void SetYaw(float yaw);
	
	float GetPitch() const { return m_Pitch; }
	void SetPitch(float pitch);

private:
	// camera options
	float m_Fov = 45.0f;
	float m_AspectRatio = 2560.f / 1440.f;

	float m_zNear = 0.1f;
	float m_zFar  = 100.f;

	float m_zNear_Ortho = 0.1f;
	float m_zFar_Ortho  = 100.f;

	float m_OrthoSize = 1.f;
	
	// euler Angles
	float m_Yaw = -90.0f;
	float m_Pitch = 0.f;

	CameraType m_CameraType = CameraType::Perspective;
};
