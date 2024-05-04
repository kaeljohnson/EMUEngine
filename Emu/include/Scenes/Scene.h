#pragma once

#include <string>

#include "../Events/EventListenerStack.h"
#include "../Events/EventListener.h"
#include "../Scenes/SceneObject.h"
#include "../Scenes/SceneObjectStack.h"
#include "../Physics/IWorld.h"

namespace Engine
{
	class Scene
	{
	public:
		int m_pixelsPerMeter;
	private:
		std::string m_name;
		float m_timeStep;
		
		SceneObjectStack m_sceneObjects;

	private:
		IWorld* m_world;

	public:
		EMU_API Scene(std::string name, const float timestep, const int pixelsPerMeter);
		EMU_API ~Scene();

		SceneObject** const begin() { return m_sceneObjects.begin(); }
		SceneObject** const end() { return m_sceneObjects.end(); }

		void checkValid();

		EMU_API void SetSimulation(const float gravityX, const float gravityY);

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		EMU_API void Add(SceneObject* sceneObject);
		EMU_API void Remove(SceneObject* sceneObject);

		void update();
	}; 
}