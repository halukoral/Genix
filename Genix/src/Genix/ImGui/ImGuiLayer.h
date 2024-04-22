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
	
	void Begin();
	void End();

	void BlockEvents(bool block) { m_BlockEvents = block; }
		
	void SetDarkThemeColors();

	uint32 GetActiveWidgetID() const;

private:
	float m_FontSize = 24.f;
	bool m_BlockEvents = true;
};
