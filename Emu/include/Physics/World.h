#pragma once

#include <memory>

#include "../MathUtil.h"

#include "PhysicsBody.h"
#include "ConversionFunctions.h"

class b2World;

namespace Engine
{
	// This class is a wrapper for the box2d b2World class. It will be used by the engine as the object where
	// all physics objects are created and updated. Scene objects which needs to adhere to physics will be added to 
	// the world object. Every scene will have one world object. So, when the client adds an object, which is a
	// box2d body, to the scene, they are by definition adding it to the world object.

	class World
	{
	private:
		// Box2D members
		b2World* m_world;
		Vector2D<float> m_gravity;

		int m_velocityIterations;
		int m_positionIterations;
	public:
		World();
		World(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations);
		~World();

		void SetGravity(const float gravityX, const float gravityY);

		void Update();
		void AddBody(std::shared_ptr<PhysicsBody> body);
		void RemoveBody(std::shared_ptr<PhysicsBody> body);
	};
}