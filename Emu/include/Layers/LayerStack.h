#pragma once

#include <vector>
#include <string>

#include "Layer.h"

namespace Engine
{

	// LayerStack class. A stack of layers that can be 
	// pushed and popped. The layer stack belongs to 
	// the current scene being updated and rendered.

	class LayerStack
	{
	private:
		std::vector<Layer*> m_layers;

	public:
		const size_t size() const;

		LayerStack();
		LayerStack(std::vector<Layer*> layers);
		~LayerStack();

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

		// Pushes a layer to the stack.
		void push(Layer* layer);

		// Pops a layer from the stack.
		void pop(Layer* layer);

		// Pops the top layer from the stack.
		void pop();
	};
}