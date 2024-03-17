#include "gxpch.h"
#include "Renderer.h"
#include "Shader.h"

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::OnWindowResize(const uint32 width, const uint32 height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const Camera& camera)
{
	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray)
{
	shader->Bind();
	shader->UploadUniform_Mat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
