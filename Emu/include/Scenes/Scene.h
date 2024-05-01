#pragma once

#include <string>

#include "../Events/EventListenerStack.h"
#include "../Events/EventListener.h"
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
		
		// Client should be able to set this.
		static const int m_maxSceneObjects = 100;
		size_t m_sceneObjectCount = 0;
		SceneObject* m_sceneObjects[m_maxSceneObjects];

	private:
		IWorld* m_world;

	public:
		Scene(std::string name, const float timestep, const int pixelsPerMeter);
		~Scene();

		void checkValid();

		EMU_API void SetSimulation(const float gravityX, const float gravityY);

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		EMU_API void Add(SceneObject* sceneObject);
		EMU_API void Remove(SceneObject* sceneObject);

		void update();
	}; 
}