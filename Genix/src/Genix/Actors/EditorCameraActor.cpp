#include "gxpch.h"
#include "Genix/Actors/EditorCameraActor.h"

#include "Genix/Common/TimeStep.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Components/Camera/EditorCameraControllerComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

EditorCameraActor* EditorCameraActor::s_Instance = nullptr;

EditorCameraActor::EditorCameraActor() : Actor("EditorCamera")
{
	m_CameraComponent = m_Entity->AddComponent<CameraComponent>();
	m_CameraControllerComponent = m_Entity->AddComponent<EditorCameraControllerComponent>();
	m_TransformComponent = m_Entity->AddComponent<TransformComponent>();

	m_CameraComponent->UpdateCameraVectors();
}

EditorCameraActor* EditorCameraActor::Get()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new EditorCameraActor();
	}
	return s_Instance;
}

void EditorCameraActor::OnUpdate(TimeStep deltaTime) const
{
	m_CameraControllerComponent->OnUpdate(deltaTime);
}

void EditorCameraActor::OnEvent(Event& e) const
{
	m_CameraControllerComponent->OnEvent(e);
}

void EditorCameraActor::OnResize(uint32 width, uint32 height) const
{
	m_CameraControllerComponent->OnResize(width, height);
}

void EditorCameraActor::SetPosition(glm::vec3 position) const
{
	m_TransformComponent->Position = position;
}

glm::mat4 EditorCameraActor::GetViewMatrix() const
{
	return m_CameraComponent->GetViewMatrix();
}

glm::mat4 EditorCameraActor::GetProjectionMatrix(CameraType cameraType) const
{
	return m_CameraComponent->GetProjectionMatrix(cameraType);
}
