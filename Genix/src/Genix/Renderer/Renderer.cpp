#include "gxpch.h"
#include "Genix/Renderer/Renderer.h"

#include "Genix/Renderer/Mesh.h"
#include "Genix/Renderer/RenderCommand.h"
#include "Genix/Renderer/Shader.h"
#include "Genix/Renderer/UniformBuffer.h"

#include "Genix/ECS/Entity.h"
#include "Genix/ECS/Components/Camera/CameraComponent.h"

Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init()
{
	GX_PROFILE_FUNCTION();

	RenderCommand::Init();
	s_SceneData->CameraUniformBuffer = UniformBuffer::Create(sizeof(SceneData::CameraData), 0);
}

void Renderer::Shutdown()
{
	GX_PROFILE_FUNCTION();
}

void Renderer::OnWindowResize(const uint32 width, const uint32 height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const Ref<Entity>& camera)
{
	GX_PROFILE_FUNCTION();

	s_SceneData->CameraBuffer.ViewProjectionMatrix = camera->GetComponent<CameraComponent>()->GetViewProjectionMatrix();
	s_SceneData->CameraBuffer.Position = camera->GetComponent<TransformComponent>()->Position;
	s_SceneData->CameraUniformBuffer->SetData(&s_SceneData->CameraBuffer, sizeof(SceneData::CameraData));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::EndScene()
{
	GX_PROFILE_FUNCTION();
}

void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, glm::mat4& transform)
{
	shader->Bind();
	shader->GLSetUniform_Mat4("u_Transform", transform);
	
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
