#pragma once
#include "Genix.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer() override;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(TimeStep deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyPressedRuntime(KeyPressedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	void NewScene();
	void OpenScene();
	void OpenScene(const std::filesystem::path& path);
	void SaveScene();
	void SaveSceneAs();
	void OnDuplicateEntity();

	void SerializeScene(const Ref<Scene>& scene, const std::filesystem::path& path);

	void OnScenePlay();
	void OnScenePause();
	void OnSceneSimulate();
	void OnSceneStop();

	// UI Panels
	void UIToolbar();
	
private:
	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	int m_GizmoType = -1;

	Ref<Actor> m_HoveredActor;

	Ref<Scene> m_ActiveScene;
	Ref<Scene> m_EditorScene;
	Ref<Scene> m_RuntimeScene;
	
	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentBrowserPanel m_ContentBrowserPanel;

	std::filesystem::path m_EditorScenePath;

	// Editor resources
	Ref<Texture> m_IconPlay;
	Ref<Texture> m_IconPause;
	Ref<Texture> m_IconSimulate;
	Ref<Texture> m_IconStep;
	Ref<Texture> m_IconStop;
	
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];
};
