#pragma once

#include "IWorld.h"

#include "box2d/box2d.h"
#include "Box.h"
#include "ConversionFunctions.h"

namespace Engine
{
	// This class is a wrapper for the box2d b2World class. It will be used by the engine as the object where
	// all physics objects are created and updated. Game objects which needs to adhere to physics will be added to 
	// the world object. Every application will have one world object. So, when the client adds an object, which is a
	// box2d body, to the application, they are by definition adding it to the world object. Essentially, the "game layer"
	// will be the layer which the world lies in. Foreground layers, background layers and UI layers will adhere to separate systems.

	class World : public IWorld
	{
	private:
		b2World m_world;
		b2Vec2 m_gravity;

		float m_deltaTime;
		int m_velocityIterations;
		int m_positionIterations;
	public:
		World();
		World(const float gravityX, const float gravityY, const float deltaTime, const int velocityIterations, const int positionIterations);
		~World();

		void update();
		void addBox(Box* body);
		void removeBox(Box* body);

		void SetGravity(const float gravityX, const float gravityY);
		void SetTimeStep(const float timeStep);
		void SetVelocityIterations(const int velocityIterations);
		void SetPositionIterations(const int positionIterations);
	};
}