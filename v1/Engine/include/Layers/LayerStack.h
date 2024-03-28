#pragma once

#include <vector>
#include <string>

#include "Layer.h"

namespace Engine
{
	class LayerStack
	{
	private:
		std::vector<Layer*> m_layers;

	public:
		const size_t size() const;

		LayerStack();
		LayerStack(std::vector<Layer*> layers);
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void popLayer();

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	};
}