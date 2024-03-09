#pragma once

#include <vector>

#include "../../include/Layers/Layer.h"
#include "../../include/Layers/LayerStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	LayerStack::LayerStack() {}
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
		m_layers.push_back(layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(std::string layerName) 
	{
		for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		{
			if ((*it)->getName() == layerName)
			{
				(*it)->onDetach();
				m_layers.erase(it);
				break;
			}
		}
	}

	void LayerStack::popLayer() 
	{
		m_layers.back()->onDetach();
		m_layers.pop_back();
	}
}