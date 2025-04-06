#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

extern const std::filesystem::path g_AssetPath;

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	const ImGuiIO& io = ImGui::GetIO();
	const auto boldFont = io.Fonts->Fonts[0];
	
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values.x = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values.y = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values.z = resetValue;
	}
	ImGui::PopFont();

	
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Ref<Entity> entity, UIFunction uiFunction)
{
	constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen |
												 ImGuiTreeNodeFlags_Framed |
												 ImGuiTreeNodeFlags_SpanAvailWidth |
												 ImGuiTreeNodeFlags_AllowItemOverlap |
												 ImGuiTreeNodeFlags_FramePadding;

	if (entity->HasComponent<T>())
	{
		auto& component = entity->GetComponent<T>();
		const ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		const bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
			{
				removeComponent = true;
			}
			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			entity->RemoveComponent<T>();
		}
	}
}

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
{
	SetScene(scene);
}

void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
{
	m_Scene = scene;
	m_SelectionContext = {};
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	for (const auto& actor : m_Scene->GetActors())
	{
		DrawActorNode(actor);
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_SelectionContext = {};
	}

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty Actor"))
		{
			m_Scene->CreateActor<Actor>();
		}
		if (ImGui::MenuItem("Create Camera Actor"))
		{
			m_Scene->CreateActor<CameraActor>();
		}
		if (ImGui::MenuItem("Create Static Mesh Actor"))
		{
			m_Scene->CreateActor<StaticMeshActor>();
		}
		if (ImGui::MenuItem("Create Basic Light Actor"))
		{
			m_Scene->CreateActor<LightActor>();
		}
		ImGui::Separator();
		
		// Basic shapes
		ImGui::MenuItem("(Basic Shapes)", nullptr, false, false);

		if (ImGui::MenuItem("Plane Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Plane";
			params.ModelPath = "Assets/Models/Basic/Plane.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		if (ImGui::MenuItem("Cube Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Cube";
			params.ModelPath = "Assets/Models/Basic/Cube.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		if (ImGui::MenuItem("Sphere Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Sphere";
			params.ModelPath = "Assets/Models/Basic/Sphere.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		if (ImGui::MenuItem("Cylinder Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Cylinder";
			params.ModelPath = "Assets/Models/Basic/Cylinder.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		if (ImGui::MenuItem("Cone Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Cone";
			params.ModelPath = "Assets/Models/Basic/Cone.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		if (ImGui::MenuItem("Monkey Actor"))
		{
			SStaticMeshSpawnParams params;
			params.Name = "Monkey";
			params.ModelPath = "Assets/Models/Basic/Monkey.fbx";
			params.VertexShaderPath = "Assets/Shaders/PBR.vert";
			params.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			
			m_Scene->CreateActor<StaticMeshActor>(params);
		}
		ImGui::EndPopup();
	}
	
	ImGui::End();

	ImGui::Begin("Properties");
	if (m_SelectionContext)
	{
		DrawComponents(m_SelectionContext->GetEntity());
	}
	ImGui::End();
}

void SceneHierarchyPanel::SetSelectedActor(Ref<Actor> actor)
{
	m_SelectionContext = actor;
}

void SceneHierarchyPanel::DrawActorNode(const Ref<Actor>& actor)
{
	ImGuiTreeNodeFlags flags = ((m_SelectionContext == actor) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	const bool isOpened = ImGui::TreeNodeEx((void*)(uint64)actor->GetEntity()->GetUUID(), flags, actor->GetEntity()->GetName().c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = actor;
	}
	
	if (isOpened)
	{
		ImGui::TreePop();	
	}
}

void SceneHierarchyPanel::DrawComponents(const Ref<Entity>& entity) const
{
	if (entity->HasComponent<TagComponent>())
	{
		auto& tag = entity->GetComponent<TagComponent>()->Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<CameraComponent>("Camera");
		DisplayAddComponentEntry<CameraControllerComponent>("Camera Controller");
		DisplayAddComponentEntry<PBRMaterialComponent>("Material");
		DisplayAddComponentEntry<TagComponent>("Tag");
		DisplayAddComponentEntry<TransformComponent>("Transform");

		ImGui::Separator();

		if (m_SelectionContext->GetEntity()->HasComponent<StaticMeshComponent>() == false)
		{
			SStaticMeshSpawnParams staticMeshSpawnParams;
			staticMeshSpawnParams.ModelPath = "Assets/Models/Basic/Cube.fbx";
			staticMeshSpawnParams.VertexShaderPath = "Assets/Shaders/PBR.vert";
			staticMeshSpawnParams.FragmentShaderPath = "Assets/Shaders/PBR.frag";
			staticMeshSpawnParams.IdForMousePick = m_SelectionContext->GetEntity()->GetIdForMousePicking();
			DisplayAddComponentEntry<StaticMeshComponent>("Static Mesh", staticMeshSpawnParams);
		}
		
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();

	DrawComponent<PBRMaterialComponent>("Material", entity, [](auto& component)
	{
		ImGui::ColorEdit3("Albedo", glm::value_ptr(component->Albedo));
		ImGui::DragFloat("Roughness", &component->Roughness, 0.05f, 0.05f, 1.0f);
		ImGui::DragFloat("Metallic", &component->Metallic, 0.05f, 0.05f, 1.0f);
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(component->EmissionColor));
		ImGui::DragFloat("Emission Power", &component->EmissionPower, 0.05f, 0.05f, FLT_MAX);
	});

	DrawComponent<LightComponent>("Light", entity, [](auto& component)
	{
		DrawVec3Control("Color", component->GetLightColor());
	});
	
	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
	{
		DrawVec3Control("Position", component->Position);
		
		glm::vec3 rotation = glm::degrees(component->Rotation);
		DrawVec3Control("Rotation", rotation);
		component->Rotation = glm::radians(rotation);

		DrawVec3Control("Scale", component->Scale, 1.0f);
	});

	// DrawComponent<StaticMeshComponent>("StaticMesh", entity, [](auto& component)
	// {
	// });

	DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
	{
		const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)component->GetCameraType()];
		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				const bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					component->SetCameraType((CameraType)i);
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (component->GetCameraType() == CameraType::Perspective)
		{
			float perspectiveVerticalFov = glm::degrees(component->GetFOV());
			if (ImGui::DragFloat("FOV", &perspectiveVerticalFov))
				component->SetFOV(glm::radians(perspectiveVerticalFov));

			float perspectiveNear = component->GetNearClip();
			if (ImGui::DragFloat("Near", &perspectiveNear))
				component->SetNearClip(perspectiveNear);

			float perspectiveFar = component->GetFarClip();
			if (ImGui::DragFloat("Far", &perspectiveFar))
				component->SetFarClip(perspectiveFar);
		}

		if (component->GetCameraType() == CameraType::Orthographic)
		{
			float orthoSize = component->GetOrthoClip();
			if (ImGui::DragFloat("Size", &orthoSize))
				component->SetOrthoClip(orthoSize);

			float orthoNear = component->GetNearClipOrtho();
			if (ImGui::DragFloat("Near", &orthoNear))
				component->SetNearClipOrtho(orthoNear);

			float orthoFar = component->GetFarClipOrtho();
			if (ImGui::DragFloat("Far", &orthoFar))
				component->SetFarClipOrtho(orthoFar);
		}
	});
}

template<typename T, typename... Args>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName, Args&&... args) const
{
	if (m_SelectionContext->GetEntity()->HasComponent<T>() == false)
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			m_SelectionContext->GetEntity()->AddComponent<T>(std::forward<Args>(args)...);
			ImGui::CloseCurrentPopup();
		}
	}
}
