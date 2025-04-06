#include "gxpch.h"
#include "Genix/ECS/Systems/RenderingManager.h"
#include "Genix/ECS/Entity.h"
#include "Genix/ECS/EntityManager.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"

void RenderingManager::Update()
{
	for (const auto& entity : EntityManager::Get()->m_Entities)
	{
		if (entity->HasComponent<StaticMeshComponent>())
		{
			const auto meshComponent = entity->GetComponent<StaticMeshComponent>();
			meshComponent->Render();
		}
	}
}
