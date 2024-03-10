#include "gxpch.h"
#include "LayerStack.h"

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
	{
		layer->OnDetach();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
	layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerStack::PopLayer(Layer* layer)
{
	const auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	const auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
	{
		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
