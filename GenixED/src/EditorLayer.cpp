#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

extern const std::filesystem::path g_AssetPath;

EditorLayer::EditorLayer() : Layer("EditorLayer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	m_IconPlay = Texture2D::Create("Assets/Editor/Icons/PlayButton.png");
	m_IconPause = Texture2D::Create("Assets/Editor/Icons/PauseButton.png");
	m_IconSimulate = Texture2D::Create("Assets/Editor/Icons/SimulateButton.png");
	m_IconStep = Texture2D::Create("Assets/Editor/Icons/StepButton.png");
	m_IconStop = Texture2D::Create("Assets/Editor/Icons/StopButton.png");
	
	m_EditorScene = CreateRef<Scene>();
	const auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
	if (commandLineArgs.Count > 1)
	{
		const auto sceneFilePath = commandLineArgs[1];
		const SceneSerializer serializer(m_EditorScene);
		serializer.Deserialize(sceneFilePath);
	}
	
	m_ActiveScene = m_EditorScene;
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(TimeStep deltaTime)
{
	// Resize
	const FramebufferSpecification spec = m_ActiveScene->GetFrameBuffer()->GetSpecification();
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f ) // zero sized framebuffer is invalid
	{
		if (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)
		{
			m_ActiveScene->GetFrameBuffer()->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
			m_ActiveScene->OnResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
		}
	}
	
	m_ActiveScene->OnUpdate(deltaTime);

	m_ActiveScene->GetFrameBuffer()->Bind();
	if (m_ActiveScene->GetSceneState() == SceneState::Edit)
	{
		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		const glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		const int mouseX = (int)mx;
		const int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			const int pixelData = m_ActiveScene->GetFrameBuffer()->ReadPixel(1, mouseX, mouseY);
			m_HoveredActor = pixelData == -1 ? nullptr : m_ActiveScene->GetActorForEditor(pixelData);
		}
	}
	m_ActiveScene->GetFrameBuffer()->Unbind();
}

void EditorLayer::OnEvent(Event& e)
{
	m_ActiveScene->OnEvent(e);
	
	if(m_ActiveScene->GetSceneState() == SceneState::Edit)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GX_BIND(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GX_BIND(EditorLayer::OnMouseButtonPressed));
	}
	else
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GX_BIND(EditorLayer::OnKeyPressedRuntime));
	}
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	// Shortcuts
	if (e.IsRepeat())
	{
		return false;
	}
	
	const bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
	const bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
	switch (e.GetKeyCode())
	{
		case Key::N:
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		case Key::O:
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case Key::S:
		{
			if (control)
			{
				if (shift)
				{
					SaveSceneAs();
				}
				else
				{
					SaveScene();
				}
			}
			break;
		}
		// Scene Commands
		case Key::Escape:
		{
			OnSceneStop();
			break;
		}
		case Key::Delete:
		{
			if (const auto actor = m_SceneHierarchyPanel.GetSelectedActor())
			{
				m_SceneHierarchyPanel.SetSelectedActor({});
				m_ActiveScene->DestroyActor(actor);
			}
			break;
		}
		case Key::D:
		{
			if (control)
			{
				OnDuplicateEntity();
			}
			break;
		}
		// Gizmos
		case Key::Q:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}
	return true;
}

bool EditorLayer::OnKeyPressedRuntime(KeyPressedEvent& e)
{
	// Shortcuts
	if (e.IsRepeat())
	{
		return false;
	}
	if (e.GetKeyCode() == Key::Escape)
	{
		OnSceneStop();		
	}
	return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Mouse::ButtonLeft)
	{
		if (m_ViewportHovered && !ImGuizmo::IsOver())
			m_SceneHierarchyPanel.SetSelectedActor(m_HoveredActor);
	}
	return false;
}

void EditorLayer::NewScene()
{
	m_EditorScene = CreateRef<Scene>();
	m_ActiveScene = m_EditorScene;
	m_ActiveScene->OnResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);
	m_EditorScenePath = std::filesystem::path();
}

void EditorLayer::OpenScene()
{
	if (const std::string filepath = FileDialogs::OpenFile("Genix Scene (*.gmap)\0*.gmap\0"); filepath.empty() == false)
	{
		OpenScene(filepath);
	}
}

void EditorLayer::OpenScene(const std::filesystem::path& path)
{
	if (path.extension().string() != ".gmap")
	{
		LOG_WARN("Could not load {0} - not a scene file", path.filename().string());
		return;
	}

	const Ref<Scene> newScene = CreateRef<Scene>();
	const SceneSerializer serializer(newScene);
	if (serializer.Deserialize(path.string()))
	{
		m_EditorScene = newScene;
		m_ActiveScene = m_EditorScene;
		m_ActiveScene->OnResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);
		m_EditorScenePath = path;
	}
}

void EditorLayer::SaveScene()
{
	if (m_EditorScenePath.empty() == false)
	{
		SerializeScene(m_ActiveScene, m_EditorScenePath);
	}
	else
	{
		SaveSceneAs();
	}
}

void EditorLayer::SaveSceneAs()
{
	const std::string filepath = FileDialogs::SaveFile("Genix Scene (*.gmap)\0*.gmap\0");
	if (filepath.empty() == false)
	{
		SerializeScene(m_ActiveScene, filepath);
		m_EditorScenePath = filepath;
	}
}

void EditorLayer::OnDuplicateEntity()
{
	if (const Ref<Actor> actor = m_SceneHierarchyPanel.GetSelectedActor())
	{
		const auto duplicatedActor = m_EditorScene->DuplicateActor(actor);
		m_SceneHierarchyPanel.SetSelectedActor(duplicatedActor);
	}
}

void EditorLayer::SerializeScene(const Ref<Scene>& scene, const std::filesystem::path& path)
{
	const SceneSerializer serializer(scene);
	serializer.Serialize(path.string());
}

void EditorLayer::OnScenePlay()
{
	m_RuntimeScene = Scene::Copy(m_EditorScene);
	m_ActiveScene = m_RuntimeScene;
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);
	m_ActiveScene->OnScenePlay();
	m_ActiveScene->OnResize();
}

void EditorLayer::OnScenePause()
{
	if (m_ActiveScene->GetSceneState() == SceneState::Edit)
	{
		return;
	}

	m_ActiveScene->SetPaused(true);
}

void EditorLayer::OnSceneSimulate()
{
	m_RuntimeScene = Scene::Copy(m_EditorScene);
	m_ActiveScene = m_RuntimeScene;
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);
	m_ActiveScene->OnSceneSimulate();
	m_ActiveScene->OnResize();
}

void EditorLayer::OnSceneStop()
{
	m_RuntimeScene->OnSceneStop();
	m_ActiveScene = m_EditorScene;
	m_ActiveScene->OnResize();
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);
	m_RuntimeScene->DestroyLevel();
	m_RuntimeScene.reset();
}

void EditorLayer::UIToolbar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	bool toolbarEnabled = (bool)m_ActiveScene;
	
	ImVec4 tintColor = ImVec4(1, 1, 1, 1);
	if (!toolbarEnabled)
	{
		tintColor.w = 0.5f;
	}

	const float size = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

	bool hasPlayButton = m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Play;
	bool hasSimulateButton = m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Simulate;
	bool hasPauseButton = m_ActiveScene->GetSceneState() != SceneState::Edit;

	if (hasPlayButton)
	{
		const Ref<Texture> icon = (m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Simulate) ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)(uint64)icon->GetId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			if (m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Simulate)
			{
				OnScenePlay();
			}
			else if (m_ActiveScene->GetSceneState() == SceneState::Play)
			{
				OnSceneStop();
			}
		}
	}
	if (hasSimulateButton)
	{
		if (hasPlayButton)
			ImGui::SameLine();

		Ref<Texture> icon = (m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Play) ? m_IconSimulate : m_IconStop;
		if (ImGui::ImageButton((ImTextureID)(uint64)icon->GetId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			if (m_ActiveScene->GetSceneState() == SceneState::Edit || m_ActiveScene->GetSceneState() == SceneState::Play)
				OnSceneSimulate();
			else if (m_ActiveScene->GetSceneState() == SceneState::Simulate)
				OnSceneStop();
		}
	}
	if (hasPauseButton)
	{
		bool isPaused = m_ActiveScene->IsPaused();
		ImGui::SameLine();
		{
			Ref<Texture> icon = m_IconPause;
			if (ImGui::ImageButton((ImTextureID)(uint64)icon->GetId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_ActiveScene->SetPaused(!isPaused);
			}
		}

		// Step button
		if (isPaused)
		{
			ImGui::SameLine();
			{
				Ref<Texture> icon = m_IconStep;
				bool isPaused = m_ActiveScene->IsPaused();
				if (ImGui::ImageButton((ImTextureID)(uint64)icon->GetId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->Step();
				}
			}
		}
	}
	
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void EditorLayer::OnImGuiRender()
{
	GX_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool optFullscreenPersistent = true;
	const bool optFullscreen = optFullscreenPersistent;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (optFullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
	ImGui::PopStyleVar();

	if (optFullscreen)
	{
		ImGui::PopStyleVar(2);
	}

	// DockSpace
	const ImGuiIO& io = ImGui::GetIO();

	ImGuiStyle& style = ImGui::GetStyle();
	const float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		const ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	style.WindowMinSize.x = minWinSizeX;
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				NewScene();
			}
			
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				OpenScene();
			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveScene();
			}
			
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}
			
			if (ImGui::MenuItem("Exit"))
			{
				Application::Get().Close();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Panels
	m_SceneHierarchyPanel.OnImGuiRender();
	m_ContentBrowserPanel.OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
	
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	const uint32 textureID = m_ActiveScene->GetFrameBuffer()->GetColorAttachmentRendererID();
	ImGui::Image((ImTextureID)(uint64)(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			OpenScene(std::filesystem::path(g_AssetPath) / path);
		}
		ImGui::EndDragDropTarget();
	}
	
	// Gizmos
	if (m_ActiveScene->GetSceneState() == SceneState::Edit)
	{
		if (const auto selectedActor = m_SceneHierarchyPanel.GetSelectedActor())
		{
			const auto selectedEntity = selectedActor->GetEntity();
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				// CameraActor
				const glm::mat4& cameraProjection = m_ActiveScene->GetProjectionMatrix();
				glm::mat4 cameraView = m_ActiveScene->GetViewMatrix();

				// Entity transform
				if (const auto transformComponent = selectedEntity->GetComponent<TransformComponent>())
				{
					auto transform = transformComponent->GetTransform();
					
					// Snapping
					const bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale
					// Snap to 45 degrees for rotation
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					const float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Math::DecomposeTransform(transform, translation, rotation, scale);

						const glm::vec3 deltaRotation = rotation - transformComponent->Rotation;
						transformComponent->Position = translation;
						transformComponent->Rotation += deltaRotation;
						transformComponent->Scale = scale;
					}
				}
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
	UIToolbar();
	ImGui::End();
}
