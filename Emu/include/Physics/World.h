#pragma once

#include <memory>

#include "IWorld.h"

#include "box2d/box2d.h"
#include "Box.h"
#include "ConversionFunctions.h"

namespace Engine
{
	// This class is a wrapper for the box2d b2World class. It will be used by the engine as the object where
	// all physics objects are created and updated. Scene objects which needs to adhere to physics will be added to 
	// the world object. Every scene will have one world object. So, when the client adds an object, which is a
	// box2d body, to the scene, they are by definition adding it to the world object.

	class World : public IWorld
	{
	private:
		// Box2D members
		b2World m_world;
		b2Vec2 m_gravity;

		int m_velocityIterations;
		int m_positionIterations;
	public:
		World();
		World(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations);
		~World();

		// Step the simulation forward.
		void update();

		// Add physics body to world.
		void addBox(std::shared_ptr<Box> body);
		
		// Remove physics body from world. 
		void removeBox(std::shared_ptr<Box> body);

		void SetGravity(const float gravityX, const float gravityY);
	};
}