#include "../../include/Clouds/Cloud.h"

#include <Engine.h>

Cloud::Cloud()
{
	/*Engine::EMU::GetInstance()->Scenes_RegisterOnPlayEvent("StartScreen", [this]() 
	{ 
			std::vector<Engine::Entity> entities = Engine::EMU::GetInstance()->Scenes_GetTileMapEntities("StartScreen", 3, 42);
			for (auto& entity : entities)
				Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, -1.0f);
	});*/
	ENGINE_CRITICAL("Creating cloud");

	Engine::EMU::GetInstance()->Scenes_AddUpdaterComponent("StartScreen", 3, 42,
		[this](Engine::Entity entity) { Update(entity); });

	/*Engine::EMU::GetInstance()->Scenes_AddPhysicsUpdaterComponent("Level1", 3, 42,
		[this](Engine::Entity entity) { Update(entity); });*/
}

// TODO: Add a Scenes_AddTransformUpdater function and remove physics from clouds. This will make it easier to control cloud movement on screen.
// Client should be able to do something like: GetScreenPosition as well so they don't have to translate world to screen coordinates themselves since
// this is especially hard when parallax effect is involved. 
void Cloud::Update(Engine::Entity entity)
{
	// CLIENT_CRITICAL("Entity: {}", entity);
	// const float cameraLeft = Engine::EMU::GetInstance()->Camera_GetOffset(playerEntity).X * 0.6f;
	// const float cameraRight = cameraLeft + Engine::EMU::GetInstance()->Camera_GetSize(playerEntity).X;

	const Math2D::Point2D<float> cloudWorldPos = Engine::EMU::GetInstance()->Transform_GetWorldPosition(entity);

	const float cloudLeft = cloudWorldPos.X;

	const float cloudRight = Engine::EMU::GetInstance()->Transform_GetWorldPosition(entity).X + 8;


	const float cloudY = Engine::EMU::GetInstance()->Transform_GetWorldPosition(entity).Y;

	// If the cloud has moved off the left side of the screen, reset its position to the right side
	//if (!Engine::EMU::GetInstance()->Camera_InFrame(entity))
	if (cloudRight < -10.0f)
	{
		Engine::EMU::GetInstance()->Transform_SetPosition(entity, Math2D::Point2D<float>(141.0f, cloudY), true);
		return;
	}

	const float interpFactor = Engine::Time::GetInterpolationFactor();

	const Math2D::Point2D<float> cloudNextWorldPos = Math2D::Point2D<float>(cloudWorldPos.X - 0.05f, cloudWorldPos.Y);

	Math2D::Point2D<float> targetPos = Math2D::Lerp(cloudWorldPos, cloudNextWorldPos, interpFactor);


	// Set the camera offset to the desired position
	Engine::EMU::GetInstance()->Transform_SetPosition(entity, cloudNextWorldPos);

	// Need to manaully set the cloud position each frame and remove it from the physics system. This will simplify 
	// cloud logic and make it easier to reset its position.
}