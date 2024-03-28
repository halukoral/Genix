#include "gxpch.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Entity.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/Shader.h"

StaticMeshComponent::StaticMeshComponent(const SStaticMeshSpawnParams& params)
	: EntityComponent(params.m_entity)
{
	m_Type = ComponentType::StaticMesh;
	m_StaticMesh = CreateRef<Model>("Assets/Models/Backpack/backpack.obj");
	m_Shader = Shader::Create("Assets/Shaders/ModelShader.vert", "Assets/Shaders/ModelShader.frag");
}


void StaticMeshComponent::Update(TimeStep deltaTime)
{
	if (m_Hide == false)
	{
		auto viewProj = Renderer::GetViewProjectionMatrix();

		Ref<EntityComponent> cmp = m_Entity->GetComponent(ComponentType::Transform);
		glm::mat4 transform = std::dynamic_pointer_cast<TransformComponent>(cmp)->GetTransform();

		m_Shader->Bind();
		m_Shader->GLSetUniform_Mat4("u_ViewProjection", viewProj );
		m_Shader->GLSetUniform_Mat4("u_Transform", transform);

		m_StaticMesh->Draw(m_Shader);	
	}
}
