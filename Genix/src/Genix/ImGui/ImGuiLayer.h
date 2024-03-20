#pragma once
#include "Genix/Layer/Layer.h"

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class WindowResizeEvent;

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Event& e) override;
	
	void OnImGuiRender() override;

	void Begin();
	void End();
	
private:
	float m_Time = 0.0f;
};
