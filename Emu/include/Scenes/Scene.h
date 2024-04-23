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

		float m_timeStep;
		int m_pixelsPerMeter;

	public:
		// Hold scenes layers.
		LayerStack m_layerStack;

		IWorld* m_world;

	public:
		Scene(std::string name, const float timestep, const int pixelsPerMeter);
		~Scene();

		EMU_API void SetSimulation(const float gravityX, const float gravityY);

		// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
		// The scene will own the layer stack.
		EMU_API void PushLayer(Layer* layer);
		EMU_API void PopLayer(Layer* layer);
		EMU_API void PopLayer();

		// Adds the layer to the world.
		EMU_API void AddToWorld(Layer* layer);

		// Removes the layer from the world.
		EMU_API void RemoveFromWorld(Layer* layer);

		void update();
	};
}