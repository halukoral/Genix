#pragma once

class StaticMeshComponent;
class TransformComponent;

class Actor
{
public:
	Actor();
	~Actor() = default;

	void SetPosition(glm::vec3 position) const;
	void SetRotation(glm::vec3 rotation) const;
	void SetScale(glm::vec3 scale) const;
	
private:
	Ref<TransformComponent> m_TransformComponent;
	Ref<StaticMeshComponent> m_StaticMeshComponent;
};
