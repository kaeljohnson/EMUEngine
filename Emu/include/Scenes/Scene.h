#pragma once

#include <string>

#include "../Layers/LayerStack.h"
#include "../Layers/Layer.h"
#include "../Physics/IWorld.h"

namespace Engine
{
	class Scene
	{
	private:
		std::string m_name;

		// Hold scenes layers.
		LayerStack m_layerStack;

		IWorld* m_world;

	public:
		Scene(std::string name);
		~Scene();

		EMU_API void SetSimulation(const float gravityX, const float gravityY, const float timeStep, const int pixelsPerMeter);

		// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
		// The scene will own the layer stack.
		EMU_API void PushToLayerStack(Layer* layer);
		EMU_API void PopLayerFromStack(Layer* layer);
		EMU_API void PopLayerFromStack();

		// Adds the layer to the world. 
		EMU_API void AddToWorld(Layer* layer);

		// Removes the layer from the world.
		EMU_API void RemoveFromWorld(Layer* layer);
	};
}