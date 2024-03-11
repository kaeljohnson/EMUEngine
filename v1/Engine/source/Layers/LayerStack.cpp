#pragma once

#include <vector>

#include "../../include/Layers/Layer.h"
#include "../../include/Layers/LayerStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	LayerStack::LayerStack() {}
	LayerStack::LayerStack(std::vector<Layer*> layers) : m_layers(layers) {}
	LayerStack::~LayerStack()
	{
		ENGINE_INFO("Freeing LayerStack.");
		for (Layer* layer : m_layers)
		{
			//delete layer;
		}
	}

	const size_t LayerStack::size() const { return m_layers.size(); }

	void LayerStack::pushLayer(Layer* layer) 
	{
		if (layer == nullptr) return;
		for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
		{
			if ((*it)->getName() == layer->getName())
			{
				ENGINE_CRITICAL("Layer with name: {} already exists in the layer stack!", layer->getName());
				return;
			}
		}

		m_layers.push_back(layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer) 
	{
		if (layer->getName() == "ApplicationLayer"
			|| layer->getName() == "WindowManagerLayer")
		{
			ENGINE_CRITICAL("Cannot pop the ApplicationLayer or WindowManagerLayer from the layer stack!");
			return;
		}

		for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		{
			if ((*it)->getName() == layer->getName())
			{
				(*it)->onDetach();
				m_layers.erase(it);
				break;
			}
		}
	}

	void LayerStack::popLayer() 
	{
		if (m_layers.back()->getName() == "ApplicationLayer"
			|| m_layers.back()->getName() == "WindowManagerLayer")
		{
			ENGINE_CRITICAL("Cannot pop the ApplicationLayer or WindowManagerLayer from the layer stack!");
			return;
		}

		m_layers.back()->onDetach();
		m_layers.pop_back();
	}
}