#include "gxpch.h"
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"

#include "Genix/Platform/OpenGL/OpenGLShader.h"

Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init()
{
	RenderCommand::Init();
}

void Renderer::Shutdown()
{
}

void Renderer::OnWindowResize(const uint32 width, const uint32 height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const Camera& camera)
{
	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, glm::mat4& transform)
{
	shader->Bind();
	shader->GLSetUniform_Mat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	shader->GLSetUniform_Mat4("u_Transform", transform);
	
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<Model>& Model, glm::mat4& transform)
{
	shader->Bind();
	shader->GLSetUniform_Mat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	shader->GLSetUniform_Mat4("u_Transform", transform);

	Model->Draw(shader);
}
