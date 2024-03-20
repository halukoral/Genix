#pragma once

#include "Genix.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer();
	~ExampleLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;
	
	void OnUpdate(TimeStep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	Ref<Shader> m_Shader;
	Ref<VertexArray> m_SquareVA;
	Ref<Texture> m_Texture;
	Ref<Texture> m_AwesomeFaceTexture;
	
	Camera m_Camera;
};
