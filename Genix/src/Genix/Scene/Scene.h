#pragma once

class Actor;
class TimeStep;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	Ref<Actor> CreateActor(glm::vec3 pos = glm::vec3{0.f}, glm::vec3 rot = glm::vec3{0.f}, glm::vec3 scale = glm::vec3{1.f});

	void Update(TimeStep deltaTime) const;
private:
	std::vector<Ref<Actor>> m_Actors;
};
