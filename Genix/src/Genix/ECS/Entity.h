#pragma once
#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Components/Camera/CameraControllerComponent.h"
#include "Genix/ECS/Components/TagComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/Core/UUID.h"

class TimeStep;

class Entity
{
public:
	using Components = std::vector<Ref<Component>>;
	using ComponentTypeMaps = std::map<const std::type_info*, Ref<Component>>;
	
	Entity() = default;
	Entity(const Entity& other);
	virtual ~Entity() { ShutDown(); }

	Entity(const std::string& name);
	Entity(const UUID& uuid, const std::string& name);

	void ShutDown();

	UUID GetUUID() const { return m_Id; }

	// Editor Only
	uint32 GetIdForMousePicking() const { return m_IdForMousePick; }
	void SetIdForMousePicking(uint32 value ) { m_IdForMousePick = value; }
	
	std::string GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	void SetSelf(Ref<Entity> self) { m_Self = self; }
	
	//! Different from hide in that the entity is not updated
	void SetInvisible(const bool invisible) { m_Invisible = invisible; }
	bool IsInvisible() const { return m_Invisible; }

	size_t GetComponentsCount() const { return m_Components.size(); }

	template <typename T, typename... Args>
	Ref<T> AddComponent(Args&&... args)
	{
		ASSERT_CORE(!HasComponent<T>(), "Entity already has component!");

		Ref<T> component = CreateRef<T>(std::forward<Args>(args)...);
		component->SetEntity(m_Self.lock());
		m_Components.push_back(component);
		m_ComponentTypeMap[&typeid(*component)] = component;
		return component;
	}

	template<typename T, typename... Args>
	Ref<T> AddOrReplaceComponent(Args&&... args)
	{
		if (HasComponent<T>())
		{
			Ref<T> component = CreateRef<T>(std::forward<Args>(args)...);
			for (int i = 0; i < (int) m_Components.size(); i++)
			{
				if (m_Components[i]->GetComponentType() == T::GetStaticType())
				{
					m_Components[i] = component;
				}
			}
			m_ComponentTypeMap[&typeid(T)] = component;
			return component;
		}
		else
		{
			return AddComponent<T>(std::forward<Args>(args)...);
		}
	}
	
	template<typename T>
	bool HasComponent() const
	{
		auto t = &typeid(T);
		return m_ComponentTypeMap.count(t);
	}

	template<typename T>
	Ref<T> GetComponent()
	{
		if (HasComponent<T>())
		{
			return std::static_pointer_cast<T>(m_ComponentTypeMap[&typeid(T)]);		
		}
		return nullptr;
	}

	template<typename T, typename... Args>
	Ref<T> GetOrCreateComponent(Args&&... args)
	{
		if (HasComponent<T>())
		{
			return std::static_pointer_cast<T>(m_ComponentTypeMap[&typeid(T)]);
		}
		else
		{
			return AddComponent<T>(std::forward<Args>(args)...);
		}
	}
	
	template<typename T>
	void RemoveComponent()
	{
		ASSERT_CORE(HasComponent<T>(), "Entity does not have component!");
		for (int i = 0; i < (int) m_Components.size(); i++)
		{
			if (m_Components[i]->GetComponentType() == T::GetStaticType())
			{
				m_Components.erase(m_Components.begin() + i);
				m_ComponentTypeMap.erase(&typeid(T));
				return;
			}
		}
	}
	
private:
	UUID m_Id;
	std::string m_Name;
	bool m_Invisible = false;

	// Editor only
	uint32 m_IdForMousePick;

	Weak<Entity> m_Self;
	
	Components m_Components;
	ComponentTypeMaps m_ComponentTypeMap;
};
