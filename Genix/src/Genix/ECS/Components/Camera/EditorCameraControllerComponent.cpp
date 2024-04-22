#include "gxpch.h"
#include "Genix/ECS/Components/Camera/EditorCameraControllerComponent.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Entity.h"

#include "Genix/Common/TimeStep.h"
#include "Genix/Events/Event.h"
#include "Genix/Events/MouseEvent.h"
#include "Genix/Input/Input.h"

void EditorCameraControllerComponent::OnUpdate(TimeStep deltaTime)
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle) || Input::IsMouseButtonPressed(Mouse::ButtonRight))
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

		if (Input::IsKeyPressed(GX_KEY_Q))
		{
			OnMove(CameraMovement::Down, deltaTime);
		}
		else if (Input::IsKeyPressed(GX_KEY_E))
		{
			OnMove(CameraMovement::Up, deltaTime);
		}
	}
}

void EditorCameraControllerComponent::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(GX_BIND(EditorCameraControllerComponent::OnMouseMoved));
}

bool EditorCameraControllerComponent::OnMouseMoved(MouseMovedEvent& e)
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		return CameraControllerComponent::OnMouseMoved(e);
	}
	else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
	{
		return CameraControllerComponent::OnMousePanned(e);
	}
	else
	{
		m_LastMouseX = e.GetX();
		m_LastMouseY = e.GetY();
	}
	return false;
}
