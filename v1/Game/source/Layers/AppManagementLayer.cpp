#pragma once

#include "../../include/Layers/AppManagementLayer.h"


AppManagementLayer::AppManagementLayer(std::string name) : Engine::Layer(name)
{
}

AppManagementLayer::~AppManagementLayer()
{
}

void AppManagementLayer::OnAttach()
{
}

void AppManagementLayer::OnDetach()
{
}

void AppManagementLayer::OnUpdate()
{
}

void AppManagementLayer::ProcessEvent(Engine::Event& e)
{
	switch (e.Type)
	{
		case (Engine::F_KEY_DOWN):
			ptrICallbackSystem->TriggerCallback(Engine::Type::ToggleFullscreen, std::monostate{});
			CLIENT_TRACE_D("Handled event: {}", static_cast<int>(Engine::F_KEY_DOWN));
			e.Handled = true;
			break;
		case (Engine::RESIZE_WINDOW):
			ptrICallbackSystem->TriggerCallback(Engine::Type::ResizeWindow, std::make_pair(e.X_POS, e.Y_POS));
			CLIENT_TRACE_D("Handled event: {}", static_cast<int>(Engine::RESIZE_WINDOW));
			e.Handled = true;
			break;
		case (Engine::ESCAPE_KEY_DOWN):
			CLIENT_TRACE_D("Handled event: {}", static_cast<int>(Engine::ESCAPE_KEY_DOWN));
			ptrICallbackSystem->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		case (Engine::QUIT):
			CLIENT_TRACE_D("Handled event: {}", static_cast<int>(Engine::QUIT));
			ptrICallbackSystem->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		default:
			break;
	}
}
