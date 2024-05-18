#pragma once

#include <memory>
#include <unordered_map>

#include "../Core.h"

#include "../Scenes/SceneObject.h"
#include "../Scenes/SceneObjectStack.h"
#include "../Physics/IWorld.h"

namespace Engine
{
	class Scene
	{
	private:
		int m_pixelsPerMeter;

		float m_gravityX;
		float m_gravityY;
		
		std::unordered_map<std::string, SceneObject*> m_sceneObjectsMap;
		SceneObjectStack m_sceneObjects;

	private:
		std::unique_ptr<IWorld> m_world;

	public:
		EMU_API Scene();
		EMU_API ~Scene() = default;

		EMU_API void SetSimulation(const float gravityX, const float gravityY, const int pixelsPerMeter);

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		EMU_API void Add(SceneObject* sceneObject);
		EMU_API void Remove(SceneObject* sceneObject);

		SceneObject** const begin() { return m_sceneObjects.begin(); }
		SceneObject** const end() { return m_sceneObjects.end(); }

		inline const int GetPixelsPerMeter() const { return m_pixelsPerMeter; }

		void checkValid();
		void update();
	}; 
}