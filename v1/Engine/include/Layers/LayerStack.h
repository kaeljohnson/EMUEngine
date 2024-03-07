#pragma once

#include <vector>

#include "Layer.h"

namespace Engine
{
	class LayerStack
	{
	private:
		std::vector<Layer*> m_layers;

	public:
		LayerStack();
		void free();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(std::string layerName);
		void popLayer();

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

		const size_t size() const;
	};
}