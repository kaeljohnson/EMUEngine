#pragma once


#include "../../include/Textures/ITexture.h"
#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture)
		: x(x), y(y), width(width), height(height), SceneObject(bodyType, true, x, y, width, height, ptrTexture)
	{
	}

	void Tile::EngineSideUpdate()
	{
		CheckAllCollisions();
	}

	void Tile::CheckAllCollisions()
	{
		m_physicsBody->SetBottomCollision(false);
		m_physicsBody->SetTopCollision(false);
		m_physicsBody->SetLeftCollision(false);
		m_physicsBody->SetRightCollision(false);

		m_physicsBody->SetBottomSensor(false);
		m_physicsBody->SetTopSensor(false);
		m_physicsBody->SetLeftSensor(false);
		m_physicsBody->SetRightSensor(false);
	}
}