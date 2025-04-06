#pragma once
#include "Genix/ECS/Component.h"

class TagComponent : public Component
{
public:
	COMPONENT_CLASS_TYPE(Tag)

	std::string Tag;

	TagComponent() = default;
	TagComponent(TagComponent& other) : Component(other), Tag(other.Tag) {}
	TagComponent(const std::string& tag) : Tag(tag) {}

	TagComponent& operator=(TagComponent& other)
	{
		m_Entity = other.m_Entity;

		Tag = other.Tag;
		return *this;
	}

};
