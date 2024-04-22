#pragma once

#include "Genix/Common/TimeStep.h"
#include "Genix/Events/Event.h"

class Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(TimeStep deltaTime) {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& event) {}
	
	const std::string& GetName() const { return m_DebugName; }

protected:
	std::string m_DebugName;
};