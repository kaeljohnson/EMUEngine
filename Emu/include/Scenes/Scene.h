#pragma once

#include <string>
#include <memory>

#include "../Events/EventListenerStack.h"
#include "../Events/EventListener.h"
#include "../Scenes/SceneObject.h"
#include "../Scenes/SceneObjectStack.h"
#include "../Physics/IWorld.h"

namespace Engine
{
	class Scene
	{
	private:
		float m_timeStep;
		int m_pixelsPerMeter;

		float m_gravityX;
		float m_gravityY;
		
		SceneObjectStack m_sceneObjects;

	private:
		std::unique_ptr<IWorld> m_world;

	public:
		EMU_API Scene();
		EMU_API ~Scene() = default;

		EMU_API void SetSimulation(const float gravityX, const float gravityY, const float timestep, const int pixelsPerMeter);

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		EMU_API void Add(SceneObject* sceneObject);
		EMU_API void Remove(SceneObject* sceneObject);

		SceneObject** const begin() { return m_sceneObjects.begin(); }
		SceneObject** const end() { return m_sceneObjects.end(); }

		inline const float GetTimeStep() const { return m_timeStep; }
		inline const int GetPixelsPerMeter() const { return m_pixelsPerMeter; }

		void checkValid();
		void update();
	}; 
}