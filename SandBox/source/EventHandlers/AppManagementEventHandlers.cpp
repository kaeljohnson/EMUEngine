 #pragma once

#include <string>

#include "../../include/EventHandlers/AppManagementEventHandlers.h"


AppManagementEventHandlers::AppManagementEventHandlers() 
{
	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::F_KEY_DOWN, [](Engine::IOEvent& e)
		{
			// Need interface to toggle fullscreen.
			Engine::EMU::GetInstance()->SetFullscreen();
			// Could add a setter for handled state and put the trace in there.
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::F_KEY_DOWN));
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::RESIZE_WINDOW, [](Engine::IOEvent& e)
		{
			// Window Interface to call the resize function.
			// refApp.GetWindowRenderer().ResizeWindow(e.X_POS, e.Y_POS);
			Engine::EMU::GetInstance()->SetWindowSize(Math2D::Point2D<int>(e.X_POS, e.Y_POS));
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::RESIZE_WINDOW));
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::ESCAPE_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::ESCAPE_KEY_DOWN));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::QUIT, [](Engine::IOEvent& e)
		{
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::QUIT));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::G_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity entity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(1, 1);
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::G_KEY_DOWN));
			Engine::EMU::GetInstance()->Camera_SetPixelsPerUnit(entity, 16);
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::H_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity entity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(1, 1);
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::H_KEY_DOWN));
			Engine::EMU::GetInstance()->Camera_SetPixelsPerUnit(entity, 32);
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::O_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_LOG_D("Handled event O KEY DOWN: {}", static_cast<int>(Engine::O_KEY_DOWN));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(2, 40);
			Engine::EMU::GetInstance()->Scenes_Deactivate(testEntity);
			CLIENT_LOG_D("Done with O key down listener");
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::O_KEY_UP, [](Engine::IOEvent& e)
		{
			Engine::Entity testEntity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(2, 40);
			Engine::EMU::GetInstance()->Scenes_Activate(testEntity);
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::B_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::B_KEY_DOWN));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(1, 1);
			Engine::EMU::GetInstance()->Physics_SetDimensions(testEntity, Math2D::Point2D<float>(0.5f, 1.7f));
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->Global_RegisterIOEventListener(Engine::N_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_LOG_D("Handled event: {}", static_cast<int>(Engine::N_KEY_DOWN));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->Scenes_GetCurrentRuntimeEntity(1, 1);
			Engine::EMU::GetInstance()->Physics_SetDimensions(testEntity, Math2D::Point2D<float>(0.5f, 0.85));
			e.Handled = true;
		});
}

AppManagementEventHandlers::~AppManagementEventHandlers() {}