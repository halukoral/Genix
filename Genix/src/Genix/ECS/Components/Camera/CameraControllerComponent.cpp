#include "gxpch.h"
#include "Genix/ECS/Components/Camera/CameraControllerComponent.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Entity.h"

#include "Genix/Common/TimeStep.h"
#include "Genix/Events/ApplicationEvent.h"
#include "Genix/Events/MouseEvent.h"

CameraControllerComponent::CameraControllerComponent(CameraControllerComponent& other) : Component(other)
{
	m_FirstMouseMove = other.m_FirstMouseMove;
	m_LastMouseX = other.m_LastMouseX;
	m_LastMouseY = other.m_LastMouseY;
	m_MovementSpeed = other.m_MovementSpeed;
	m_PanSpeed = other.m_PanSpeed;
	m_MouseSensitivity = other.m_MouseSensitivity;

	m_ViewportSize = other.m_ViewportSize;
}

CameraControllerComponent& CameraControllerComponent::operator=(CameraControllerComponent& other)
{
	m_Entity = other.m_Entity;

	m_FirstMouseMove = other.m_FirstMouseMove;
	m_LastMouseX = other.m_LastMouseX;
	m_LastMouseY = other.m_LastMouseY;
	m_MovementSpeed = other.m_MovementSpeed;
	m_PanSpeed = other.m_PanSpeed;
	m_MouseSensitivity = other.m_MouseSensitivity;

	m_ViewportSize = other.m_ViewportSize;
	return *this;
}

void CameraControllerComponent::OnUpdate(TimeStep deltaTime)
{
	if (Input::IsKeyPressed(GX_KEY_A))
	{
		OnMove(CameraMovement::Left, deltaTime);
	}
	else if (Input::IsKeyPressed(GX_KEY_D))
	{
		OnMove(CameraMovement::Right, deltaTime);
	}

	if (Input::IsKeyPressed(GX_KEY_W))
	{
		OnMove(CameraMovement::Forward, deltaTime);
	}
	else if (Input::IsKeyPressed(GX_KEY_S))
	{
		OnMove(CameraMovement::Backward, deltaTime);
	}
}

void CameraControllerComponent::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(GX_BIND(CameraControllerComponent::OnMouseMoved));
	dispatcher.Dispatch<MouseScrolledEvent>(GX_BIND(CameraControllerComponent::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(GX_BIND(CameraControllerComponent::OnWindowResized));
}


void CameraControllerComponent::OnMove(const CameraMovement direction, TimeStep deltaTime) const
{
	const float velocity = m_MovementSpeed * deltaTime;
	SetPosition(direction, velocity);
}

void CameraControllerComponent::OnResize(const uint32 width, const uint32 height)
{
	m_ViewportSize.x = width;
	m_ViewportSize.y = height;
	if (const auto& cmp = GetEntity()->GetComponent<CameraComponent>())
	{
		cmp->SetAspectRatio((float)width / (float)height);
	}
}

void CameraControllerComponent::SetPosition(CameraMovement direction, float velocity) const
{
	if (const auto& cmp = GetEntity()->GetComponent<TransformComponent>())
	{
		if (direction == CameraMovement::Up)
		{
			cmp->Position += cmp->Up * velocity;
		}
		if (direction == CameraMovement::Down)
		{
			cmp->Position -= cmp->Up * velocity;
		}
		if (direction == CameraMovement::Forward)
		{
			cmp->Position += cmp->Front * velocity;
		}
		if (direction == CameraMovement::Backward)
		{
			cmp->Position -= cmp->Front * velocity;
		}
		if (direction == CameraMovement::Left)
		{
			cmp->Position -= cmp->Right * velocity;
		}
		if (direction == CameraMovement::Right)
		{
			cmp->Position += cmp->Right * velocity;
		}
	}
}

bool CameraControllerComponent::OnMouseMoved(MouseMovedEvent& e)
{
	if (m_FirstMouseMove)
	{
		m_LastMouseX = e.GetX();	
		m_LastMouseY = e.GetY();
		m_FirstMouseMove = false;
	}
	
	float xOffset = e.GetX() - m_LastMouseX;
	float yOffset = m_LastMouseY - e.GetY(); // reversed since y-coordinates go from bottom to top

	m_LastMouseX = e.GetX();
	m_LastMouseY = e.GetY();
	
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	if (const auto& cmp = GetEntity()->GetComponent<CameraComponent>())
	{
		cmp->SetYaw(xOffset);
		cmp->SetPitch(yOffset);
		cmp->UpdateCameraVectors();
	}

	return false;
}

bool CameraControllerComponent::OnMousePanned(MouseMovedEvent& e)
{
	if (m_FirstMouseMove)
	{
		m_LastMouseX = e.GetX();	
		m_LastMouseY = e.GetY();
		m_FirstMouseMove = false;
	}
	
	float xOffset = e.GetX() - m_LastMouseX;
	float yOffset = m_LastMouseY - e.GetY(); // reversed since y-coordinates go from bottom to top

	m_LastMouseX = e.GetX();
	m_LastMouseY = e.GetY();

	xOffset *= m_PanSpeed;
	yOffset *= m_PanSpeed;

	SetPosition(CameraMovement::Right, xOffset);
	SetPosition(CameraMovement::Up, yOffset);
	
	return false;
}

bool CameraControllerComponent::OnMouseScrolled(MouseScrolledEvent& e)
{
	if (const auto& cmp = GetEntity()->GetComponent<CameraComponent>())
	{
		cmp->SetFOV(e.GetYOffset() * 0.25f * -1.f);
	}
	
	return false;
}

bool CameraControllerComponent::OnWindowResized(WindowResizeEvent& e)
{
	if (const auto& cmp = GetEntity()->GetComponent<CameraComponent>())
	{
		cmp->SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
	}
	return false;
}
