#pragma once

#include "Genix/Core.h"
#include "Genix/Events/Event.h"

class GENIX_API Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& event) {}
	
	const std::string& GetName() const { return m_DebugName; }

protected:
	std::string m_DebugName;
};