#include "gxpch.h"
#include "Renderer.h"

void Renderer::OnWindowResize(uint32 width, uint32 height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{
}

void Renderer::SubmitVAO(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
