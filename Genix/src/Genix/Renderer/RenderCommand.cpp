#include "gxpch.h"
#include "Genix/Renderer/RenderCommand.h"

Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<RendererAPI>();

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

void RenderCommand::SetActiveTexture(const uint8 slot)
{
	s_RendererAPI->SetActiveTexture(slot);
}

void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
	s_RendererAPI->DrawIndexed(vertexArray);
}

void RenderCommand::ResetFrameBuffer()
{
	s_RendererAPI->ResetFrameBuffer();
}

void RenderCommand::SetBlendFunc(BlendFuncArgument arg1, BlendFuncArgument arg2)
{
	s_RendererAPI->SetBlendFunc(arg1, arg2);
}

void RenderCommand::RenderCube()
{
	s_RendererAPI->RenderCube();
}

void RenderCommand::RenderSphere()
{
	s_RendererAPI->RenderSphere();
}

void RenderCommand::RenderQuad()
{
	s_RendererAPI->RenderQuad();
}
