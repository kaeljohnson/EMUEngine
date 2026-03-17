#include "../../include/Clouds/Cloud.h"

#include <Engine.h>

Cloud::Cloud()
{
	Engine::EMU::GetInstance()->Scenes_RegisterOnPlayEvent("StartScreen", [this]() 
	{ 
			std::vector<Engine::Entity> entities = Engine::EMU::GetInstance()->Scenes_GetTileMapEntities("StartScreen", 3, 42);
			for (auto& entity : entities)
				Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, -1.0f);
	});

	Engine::EMU::GetInstance()->Scenes_AddPhysicsUpdaterComponent("StartScreen", 3, 42,
		[this](Engine::Entity entity) { Update(entity); });
}

// TODO: Add a Scenes_AddTransformUpdater function and remove physics from clouds. This will make it easier to control cloud movement on screen.
// Client should be able to do something like: GetScreenPosition as well so they don't have to translate world to screen coordinates themselves since
// this is especially hard when parallax effect is involved. 
void Cloud::Update(Engine::Entity entity)
{
	Engine::Entity playerEntity = Engine::EMU::GetInstance()->Scenes_GetTileMapEntity("StartScreen", 1, 1);
	const float cameraLeft = Engine::EMU::GetInstance()->Camera_GetOffset(playerEntity).X;
	const float cameraRight = cameraLeft + Engine::EMU::GetInstance()->Camera_GetSize(playerEntity).X;

	const float cloudLeft = Engine::EMU::GetInstance()->Transform_GetPosition(entity).X;
	const float cloudRight = cloudLeft + Engine::EMU::GetInstance()->Physics_GetDimensions(entity).X;

	const float cloudY = Engine::EMU::GetInstance()->Transform_GetPosition(entity).Y;

	CLIENT_CRITICAL_D("Camera left: {}, size: {}", cameraLeft, cameraRight);
	CLIENT_CRITICAL_D("Cloud left on screen: {}, size: {}", cloudLeft, cloudRight);

	// If the cloud has moved off the left side of the screen, reset its position to the right side
	if (!Engine::EMU::GetInstance()->Camera_InFrame(entity))
	{
		Engine::EMU::GetInstance()->Physics_SetPosition(entity, Math2D::Point2D<float>(cameraRight, cloudY));
	}

	/*if (cloudLeft > cameraRight)
	{
		Engine::EMU::GetInstance()->Physics_SetPosition(entity, -100.0f);
	}*/
}