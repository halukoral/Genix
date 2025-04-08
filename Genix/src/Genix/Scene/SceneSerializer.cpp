#include "gxpch.h"
#include "Genix/Scene/SceneSerializer.h"
#include "Genix/Scene/Scene.h"

#include "Genix/Actors/Actor.h"

#include "Genix/ECS/Entity.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/ECS/Components/TagComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include "Genix/ECS/Components/LightComponent.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, const Ref<Entity>& entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity->GetUUID();
		out << YAML::Key << "Name" << YAML::Value << entity->GetName(); 

		if (entity->HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			const auto& cameraComponent = entity->GetComponent<CameraComponent>();
			out << YAML::Key << "Fov" << YAML::Value << cameraComponent->GetFOV();
			out << YAML::Key << "AspectRatio" << YAML::Value << cameraComponent->GetAspectRatio();
			out << YAML::Key << "Near" << YAML::Value << cameraComponent->GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << cameraComponent->GetFarClip();
			out << YAML::Key << "NearO" << YAML::Value << cameraComponent->GetNearClipOrtho();
			out << YAML::Key << "FarO" << YAML::Value << cameraComponent->GetFarClipOrtho();
			out << YAML::Key << "OrthoSize" << YAML::Value << cameraComponent->GetOrthoClip();
			out << YAML::Key << "Yaw" << YAML::Value << cameraComponent->GetYaw();
			out << YAML::Key << "Pitch" << YAML::Value << cameraComponent->GetPitch();
			out << YAML::Key << "Type" << YAML::Value << (int)cameraComponent->GetCameraType();

			out << YAML::EndMap; // CameraComponent
		}

		if (entity->HasComponent<CameraControllerComponent>())
		{
			out << YAML::Key << "CameraControllerComponent";
			out << YAML::BeginMap; // CameraControllerComponent

			const auto& cameraControllerComponent = entity->GetComponent<CameraControllerComponent>();
			out << YAML::Key << "FirstMouse" << YAML::Value << cameraControllerComponent->GetMouseFirstMove();
			out << YAML::Key << "MouseX" << YAML::Value << cameraControllerComponent->GetMousePos().first;
			out << YAML::Key << "MouseY" << YAML::Value << cameraControllerComponent->GetMousePos().second;
			out << YAML::Key << "MouseSpeed" << YAML::Value << cameraControllerComponent->GetMovementSpeed();
			out << YAML::Key << "PanSpeed" << YAML::Value << cameraControllerComponent->GetPanSpeed();
			out << YAML::Key << "MouseSensitivity" << YAML::Value << cameraControllerComponent->GetMouseSensitivity();

			out << YAML::EndMap; // CameraControllerComponent
		}
		
		if (entity->HasComponent<LightComponent>())
		{
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap; // LightComponent

			const auto& lightComponent = entity->GetComponent<LightComponent>();
			out << YAML::Key << "LightColor" << YAML::Value << lightComponent->GetLightColor();

			out << YAML::EndMap; // LightComponent
		}
	
		if (entity->HasComponent<StaticMeshComponent>())
		{
			out << YAML::Key << "StaticMeshComponent";
			out << YAML::BeginMap; // StaticMeshComponent

			const auto& staticMeshComponent = entity->GetComponent<StaticMeshComponent>();
			out << YAML::Key << "ModelPath" << YAML::Value << staticMeshComponent->GetModelPath();
			out << YAML::Key << "VertexShaderPath" << YAML::Value << staticMeshComponent->GetVertexShaderPath();
			out << YAML::Key << "FragmentShaderPath" << YAML::Value << staticMeshComponent->GetFragmentShaderPath();

			out << YAML::Key << "Hide" << YAML::Value << staticMeshComponent->IsHidden();

			out << YAML::EndMap; // StaticMeshComponent
		}
		
		if (entity->HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			const auto& tag = entity->GetComponent<TagComponent>()->Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity->HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			const auto& transformComponent = entity->GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComponent->Position;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent->Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent->Scale;

			out << YAML::Key << "WorldUp" << YAML::Value << transformComponent->WorldUp;
			out << YAML::Key << "Up" << YAML::Value << transformComponent->Up;
			out << YAML::Key << "Front" << YAML::Value << transformComponent->Front;
			out << YAML::Key << "Right" << YAML::Value << transformComponent->Right;

			out << YAML::EndMap; // TransformComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath) const
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (const auto& actor : m_Scene->GetActors())
		{
			if (actor->GetEntity())
			{
				SerializeEntity(out, actor->GetEntity());
			}
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
	}

	bool SceneSerializer::Deserialize(const std::string& filepath) const
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			LOG_CORE_ERROR("Failed to load .gmap file '{0}'\n     {1}", filepath, e.what());
			return false;
		}
	
		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		LOG_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		if (auto entities = data["Entities"])
		{
			for (auto entity : entities)
			{
				uint64 uuid = entity["Entity"].as<uint64>();

				std::string name = entity["Name"].as<std::string>();

				LOG_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				auto deserializedActor = m_Scene->CreateActor<Actor>(GUUID(uuid), name);
				auto deserializedEntity = deserializedActor->GetEntity();

				if (auto node = entity["CameraComponent"])
				{
					if (auto cameraComponent = deserializedEntity->AddComponent<CameraComponent>())
					{
						cameraComponent->SetFOV(node["Fov"].as<float>());
						cameraComponent->SetAspectRatio(node["AspectRatio"].as<float>());
						cameraComponent->SetNearClip(node["Near"].as<float>());
						cameraComponent->SetFarClip(node["Far"].as<float>());
						cameraComponent->SetNearClipOrtho(node["NearO"].as<float>());
						cameraComponent->SetFarClipOrtho(node["FarO"].as<float>());
						cameraComponent->SetOrthoClip(node["OrthoSize"].as<float>());
						cameraComponent->SetYaw(node["Yaw"].as<float>());
						cameraComponent->SetPitch(node["Pitch"].as<float>());
						cameraComponent->SetCameraType((CameraType)node["Type"].as<int>());
					}
				}
				
				if (auto node = entity["CameraControllerComponent"])
				{
					if (auto cameraControllerComponent = deserializedEntity->AddComponent<CameraControllerComponent>())
					{
						cameraControllerComponent->SetMouseFirstMove(node["FirstMouse"].as<bool>());
						cameraControllerComponent->SetMousePos(node["MouseX"].as<std::pair<float,float>>());
						cameraControllerComponent->SetMousePos(node["MouseY"].as<std::pair<float,float>>());
						cameraControllerComponent->SetMovementSpeed(node["MouseSpeed"].as<float>());
						cameraControllerComponent->SetPanSpeed(node["PanSpeed"].as<float>());
						cameraControllerComponent->SetMouseSensitivity(node["MouseSensitivity"].as<float>());
					}
				}
				
				if (auto node = entity["LightComponent"])
				{
					if (auto lightComponent = deserializedEntity->AddComponent<LightComponent>())
					{
						lightComponent->SetLightColor(node["LightColor"].as<glm::vec3>());
					}
				}
				
				if (auto node = entity["StaticMeshComponent"])
				{
					SStaticMeshSpawnParams params;
					params.ModelPath = node["ModelPath"].as<std::string>();
					params.VertexShaderPath = node["VertexShaderPath"].as<std::string>();
					params.FragmentShaderPath = node["FragmentShaderPath"].as<std::string>();

					if (auto staticMeshComponent = deserializedEntity->AddComponent<StaticMeshComponent>(params))
					{
						staticMeshComponent->SetHide(node["Hide"].as<bool>());
					}
				}
				
				if (auto node = entity["TagComponent"])
				{
					if (auto tagComponent = deserializedEntity->AddComponent<TagComponent>())
					{
						tagComponent->Tag = node["Tag"].as<std::string>();
					}
				}
				
				if (auto node = entity["TransformComponent"])
				{
					if (auto transformComponent = deserializedEntity->AddComponent<TransformComponent>())
					{
						transformComponent->Position	= node["Translation"].as<glm::vec3>();
						transformComponent->Rotation	= node["Rotation"].as<glm::vec3>();
						transformComponent->Scale		= node["Scale"].as<glm::vec3>();
						transformComponent->WorldUp		= node["WorldUp"].as<glm::vec3>();
						transformComponent->Up			= node["Up"].as<glm::vec3>();
						transformComponent->Front		= node["Front"].as<glm::vec3>();
						transformComponent->Right		= node["Right"].as<glm::vec3>();
					}
				}
			}
		}
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		LOG_CORE_TRACE(false);
		return false;
	}
