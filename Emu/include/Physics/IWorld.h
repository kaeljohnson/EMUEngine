#pragma once

#include <memory>

// World Interface for Box2D World.

namespace Engine
{
	class PhysicsBody;

	// Client will not be able to create an instance of this class in the future.
	class IWorld
	{
	public:
		virtual ~IWorld() = default;
		virtual void Update() = 0;
		virtual void AddBody(std::shared_ptr<PhysicsBody> body) = 0;
		virtual void RemoveBody(std::shared_ptr<PhysicsBody> body) = 0;

		virtual void SetGravity(const float gravityX, const float gravityY) = 0;
	};
}