#pragma once

#include <string>
#include <vector>

#include "../Layers/LayerStack.h"
#include "../Layers/Layer.h"
#include "../Scenes/SceneObject.h"
#include "../Physics/IWorld.h"

namespace Engine
{
	class Scene
	{
	public:
		std::string m_name;

		float m_timeStep;
		int m_pixelsPerMeter;

	private:
		// Hold scenes layers.
		LayerStack m_layerStack;

		// Change this to better structure.
		std::vector<SceneObject*> m_sceneObjects;

		IWorld* m_world;

	public:
		Scene(std::string name, const float timestep, const int pixelsPerMeter);
		~Scene();

		void checkValid();

		std::vector<Layer*>::iterator begin() { return m_layerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_layerStack.end(); }

		EMU_API void SetSimulation(const float gravityX, const float gravityY);

		// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
		// The scene will own the layer stack.
		EMU_API void PushLayer(Layer* layer);

		// Adds scene object to the scene, and ties it to a layer.
		// Scene objects must be tied to a layer. For now.
		EMU_API void Add(SceneObject* sceneObject, Layer* layerName);
		//void AddToWorld(Layer* layer);
		EMU_API void PopLayer(Layer* layer);
		EMU_API void PopLayer();

		// Adds the layer to the world.
		//EMU_API void AddToWorld(Layer* layer);

		// Removes the layer from the world.
		EMU_API void RemoveFromWorld(Layer* layer);

		void update();
	}; 
}