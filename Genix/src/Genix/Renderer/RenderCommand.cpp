#include "gxpch.h"
#include "RenderCommand.h"

#include "Genix/Platform/OpenGL/OpenGLRendererAPI.h"

Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

void RenderCommand::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	s_RendererAPI->SetViewport(x, y, width, height);
}

void RenderCommand::SetClearColor(const glm::vec4& color)
{
	s_RendererAPI->SetClearColor(color);
}

void RenderCommand::Clear()
{
	s_RendererAPI->Clear();
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	s_RendererAPI->DrawIndexed(vertexArray);
}
