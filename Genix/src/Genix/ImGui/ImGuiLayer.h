#pragma once
#include "Genix/Layer.h"

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class WindowResizeEvent;

class GENIX_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void OnImGuiRender() override;

	void Begin();
	void End();
	
private:
	float m_Time = 0.0f;
};
