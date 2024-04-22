#include "gxpch.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"

#include "PBRMaterialComponent.h"
#include "Genix/ECS/Components/LightComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Entity.h"
#include "Genix/ECS/EntityManager.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/Shader.h"

StaticMeshComponent::~StaticMeshComponent()
{
	m_StaticMesh.reset();
	m_Shader.reset();
}

StaticMeshComponent::StaticMeshComponent(const SStaticMeshSpawnParams& params)
	: m_ModelPath(params.ModelPath),
	m_VertexShaderPath(params.VertexShaderPath), m_FragmentShaderPath(params.FragmentShaderPath)
{
	m_StaticMesh = CreateRef<Mesh>(params.ModelPath, params.IdForMousePick);
	m_Shader = Shader::Create(params.VertexShaderPath, params.FragmentShaderPath);
}

StaticMeshComponent::StaticMeshComponent(StaticMeshComponent& other) : Component(other)
{
	m_StaticMesh = CreateRef<Mesh>(other.GetModelPath(), other.GetEntity()->GetIdForMousePicking());
	m_Shader = Shader::Create(other.GetVertexShaderPath(), other.GetFragmentShaderPath());
}

void StaticMeshComponent::Render()
{
	if (m_Hide == false && m_StaticMesh && m_Shader)
	{
		const auto viewProj = Renderer::GetViewProjectionMatrix();
		const glm::vec3 camPosition = Renderer::GetCameraPosition();
		const glm::mat4 transform = GetEntity()->GetComponent<TransformComponent>()->GetTransform();
		
		m_Shader->Bind();
		m_Shader->GLSetUniform_Mat4("u_ViewProjection", viewProj);
		m_Shader->GLSetUniform_Mat4("u_Transform", transform);
		m_Shader->GLSetUniform_Vec3("u_ViewPos", camPosition);
		m_Shader->GLSetUniform_Mat3("u_Normal", glm::transpose(glm::inverse(glm::mat3(transform))));
		
		if (const auto& cmp = GetEntity()->GetComponent<PBRMaterialComponent>())
		{
			m_Shader->GLSetUniform_Vec3("albedo", cmp->Albedo);
			m_Shader->GLSetUniform_Float("metallic", cmp->Metallic);
			m_Shader->GLSetUniform_Float("roughness", cmp->Roughness);
			m_Shader->GLSetUniform_Float("ao", 1.0f);

			m_Shader->GLSetUniform_Int("irradianceMap", 0);
			m_Shader->GLSetUniform_Int("prefilterMap", 1);
			m_Shader->GLSetUniform_Int("brdfLUT", 2);
		}

		if (const auto& entity = EntityManager::Get()->GetFirstEntityByComponent<LightComponent>())
		{
			if (const auto& cmp = entity->GetComponent<LightComponent>())
			{
				m_Shader->GLSetUniform_Vec3("lightPosition", cmp->GetLightPos());
				m_Shader->GLSetUniform_Vec3("lightColor", cmp->GetLightColor());
			}
		}
		
		m_StaticMesh->Draw(m_Shader);
		m_Shader->Unbind();
	}
}
