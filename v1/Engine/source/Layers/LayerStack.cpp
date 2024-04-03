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
		// Once layers are pushed to the applications layer stack, the application takes the
		// ownership of the layer and therefore the responsibility of deleting the layers.
		// If the client pops the layer from the stack, the client is responsible for 
		// deleting the layer.

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
			if ((*it)->GetName() == layer->GetName())
			{
				ENGINE_CRITICAL("Layer with name: {} already exists in the layer stack!", layer->GetName());
				return;
			}
		}

		// Stuff engine needs to do when a layer is pushed.
		// Game objects added to the application world when layer is pushed to application layer stack.
		layer->AddToWorld();

		// Stuff the client needs to do when a layer is pushed.
		layer->OnAttach();

		// Add the layer to the layer stack.
		m_layers.push_back(layer);
	}

	void LayerStack::popLayer(Layer* layer) 
	{
		for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		{
			if ((*it)->GetName() == layer->GetName())
			{

				// Stuff engine needs to do when a layer is popped.
				(*it)->RemoveFromWorld();

				// Stuff the client needs to do when a layer is popped.
				(*it)->OnDetach();

				// Remove the layer from the layer stack.
				m_layers.erase(it);
				break;
			}
		}
	}

	void LayerStack::popLayer() 
	{
		// Stuff engine needs to do when a layer is popped.
		m_layers.back()->RemoveFromWorld();

		// Stuff the client needs to do when a layer is popped.
		m_layers.back()->OnDetach();

		// Remove the layer from the layer stack.
		m_layers.pop_back();
	}
}