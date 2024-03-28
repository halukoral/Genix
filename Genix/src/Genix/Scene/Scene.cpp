#include "gxpch.h"
#include "Genix/Scene/Scene.h"
#include "Genix/Actors/Actor.h"
#include "Genix/Common/TimeStep.h"
#include "Genix/ECS/EntityManager.h"

Ref<Actor> Scene::CreateActor(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	const Ref<Actor> actor = CreateRef<Actor>();
	actor->SetPosition(pos);
	actor->SetRotation(rot);
	actor->SetScale(scale);
	
	m_Actors.push_back(actor);
	return actor;
}

void Scene::Update(TimeStep deltaTime) const
{
	EntityManager::Get()->Update(deltaTime);
}
