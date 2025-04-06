#include "gxpch.h"
#include "Genix/Actors/CameraActor.h"

#include "Genix/Common/TimeStep.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Components/Camera/CameraControllerComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

CameraActor::CameraActor() : Actor(s_Name + std::to_string(s_CameraActorCount++))
{
	GX_PROFILE_FUNCTION();

	m_CameraComponent = m_Entity->AddComponent<CameraComponent>();
	m_CameraControllerComponent = m_Entity->AddComponent<CameraControllerComponent>();
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();

	m_CameraComponent->UpdateCameraVectors();
}

CameraActor::CameraActor(const std::string& name) : Actor(name)
{
	GX_PROFILE_FUNCTION();

	m_CameraComponent = m_Entity->AddComponent<CameraComponent>();
	m_CameraControllerComponent = m_Entity->AddComponent<CameraControllerComponent>();
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();

	m_CameraComponent->UpdateCameraVectors();
}

CameraActor::~CameraActor()
{
	m_CameraComponent.reset();
	m_CameraControllerComponent.reset();
	m_TransformComponent.reset();
}

CameraActor::CameraActor(CameraActor& other)
{
	*m_CameraComponent = *other.m_CameraComponent;
	*m_CameraControllerComponent = *other.m_CameraControllerComponent;
	*m_TransformComponent = *other.m_TransformComponent;
	m_CameraComponent->UpdateCameraVectors();
}

CameraActor& CameraActor::operator=(CameraActor& other)
{
	*m_CameraComponent = *other.m_CameraComponent;
	*m_CameraControllerComponent = *other.m_CameraControllerComponent;
	*m_TransformComponent = *other.m_TransformComponent;
	m_CameraComponent->UpdateCameraVectors();
	return *this;
}

void CameraActor::OnUpdate(TimeStep deltaTime) const
{
	m_CameraControllerComponent->OnUpdate(deltaTime);
}

void CameraActor::OnEvent(Event& e) const
{
	m_CameraControllerComponent->OnEvent(e);
}

void CameraActor::OnResize(const uint32 width, const uint32 height) const
{
	m_CameraControllerComponent->OnResize(width, height);
}

void CameraActor::SetPosition(const glm::vec3 position) const
{
	m_TransformComponent->Position = position;
}

glm::mat4 CameraActor::GetViewMatrix() const
{
	return m_CameraComponent->GetViewMatrix();
}

glm::mat4 CameraActor::GetProjectionMatrix(const CameraType cameraType) const
{
	return m_CameraComponent->GetProjectionMatrix(cameraType);
}
