#include "gxpch.h"
#include "Renderer.h"
#include "Shader.h"

#include "Genix/Platform/OpenGL/OpenGLShader.h"

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

void Renderer::OnWindowResize(const uint32 width, const uint32 height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const Camera& camera)
{
	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray, glm::mat4& transform)
{
	shader->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform_Mat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform_Mat4("u_Transform", transform);
	
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
