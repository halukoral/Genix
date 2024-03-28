#pragma once

#include "Genix.h"

class ModelLayer : public Layer
{
public:
	ModelLayer();
	~ModelLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;
	
	void OnUpdate(TimeStep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	Ref<Shader> m_ModelShader;
	Ref<Model> m_Model;
	
	Camera m_Camera;
};
