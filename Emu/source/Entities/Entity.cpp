#pragma once

#include <memory>

#include "../../include/Entities/Entity.h"
#include "../../include/Scenes/SceneObject.h"

namespace Engine 
{
	Entity::Entity(const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture)
		: 
		// OnCelingFlag(false), OnLeftWallFlag(false), OnRightWallFlag(false), OnGroundFlag(false),
		SceneObject(DYNAMIC, false, x, y, width, height, ptrTexture)
	{}

	void Entity::CheckAllCollisions()
	{
		//m_physicsBody->SetIsBeingCollidedWith(false);
		//m_physicsBody->SetIsCollidingWith(false);

		//OnGroundFlag = m_physicsBody->OnGround();
		//// m_physicsBody->OnCeling();
		//// m_physicsBody->OnLeftWall();
		//// m_physicsBody->OnRightWall();

		//m_physicsBody->SetIsCollidingWith(OnGroundFlag || OnCelingFlag || OnLeftWallFlag || OnRightWallFlag);

		m_physicsBody->SetCollisionFlags();
		// OnGroundFlag = m_physicsBody->GetHasBottomCollision();
	}
}