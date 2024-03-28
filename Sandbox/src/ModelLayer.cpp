#include "ModelLayer.h"

ModelLayer::ModelLayer()
{
	m_Model = CreateRef<Model>("Assets/Models/Backpack/backpack.obj");
	m_ModelShader = Shader::Create("Assets/Shaders/ModelShader.vert", "Assets/Shaders/ModelShader.frag");
}

void ModelLayer::OnAttach()
{
}

void ModelLayer::OnDetach()
{
	
}

void ModelLayer::OnUpdate(TimeStep deltaTime)
{
	m_Camera.OnUpdate(deltaTime);
		
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_Camera);

	Renderer::Submit(m_ModelShader, m_Model);
		
	Renderer::EndScene();
}

void ModelLayer::OnImGuiRender()
{
}

void ModelLayer::OnEvent(Event& e)
{
	m_Camera.OnEvent(e);
}
