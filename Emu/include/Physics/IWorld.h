#pragma once

#include <memory>

// World Interface for Box2D World.

namespace Engine
{
	class Box;

	// Client will not be able to create an instance of this class in the future.
	class IWorld
	{
	public:
		virtual ~IWorld() = default;
		virtual void update() = 0;
		virtual void addBox(std::shared_ptr<Box> body) = 0;
		virtual void removeBox(std::shared_ptr<Box> body) = 0;

		virtual void SetGravity(const float gravityX, const float gravityY) = 0;
	};
}