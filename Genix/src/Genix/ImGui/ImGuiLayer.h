#pragma once
#include "Genix/Layer.h"

class GENIX_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Event& event) override;
	
private:
	float m_Time = 0.0f;
};
