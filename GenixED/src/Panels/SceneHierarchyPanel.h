#pragma once
#include "Genix.h"

class Actor;
class Scene;

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Ref<Scene>& scene);

	void SetScene(const Ref<Scene>& scene);
	void OnImGuiRender();

	Ref<Actor> GetSelectedActor() const { return m_SelectionContext; }
	void SetSelectedActor(Ref<Actor> actor);

private:
	template<typename T, typename... Args>
	void DisplayAddComponentEntry(const std::string& entryName, Args&&... args) const;
	
	void DrawActorNode(const Ref<Actor>& actor);
	void DrawComponents(const Ref<Entity>& entity) const;

	Ref<Scene> m_Scene;
	Ref<Actor> m_SelectionContext;
};
