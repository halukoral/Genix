#pragma once
#include "Genix.h"

class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer() override;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(TimeStep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;
	
private:
	Camera m_Camera;
	Ref<Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
};
