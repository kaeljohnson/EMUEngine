#pragma once

#include "../include/WindowManagerLayer.h"

namespace Engine
{
	WindowManagerLayer::WindowManagerLayer(IEventSystem& eventSystem) : refEventSystem(eventSystem), Layer("WindowManagerLayer")
	{
	}

	WindowManagerLayer::~WindowManagerLayer()
	{
	}

	void WindowManagerLayer::onAttach()
	{
	}

	void WindowManagerLayer::onDetach()
	{
	}

	void WindowManagerLayer::onUpdate()
	{
	}

	void WindowManagerLayer::processEvent(Event& e)
	{
	
		switch (e.m_eventType)
		{
			case (F_KEY_DOWN): 
				/*ptrWindowManager->toggleFullscreen();*/ 
				refEventSystem.triggerEventCallback(ActionType::ToggleFullscreen); 
				ENGINE_TRACE("Handled event: {}", static_cast<int>(F_KEY_DOWN)); 
				e.handled = true; 
				break;
			case (RESIZE_WINDOW): 
				/*ptrWindowManager->resize(e.m_xPos, e.m_yPos);*/ 
				//refEventSystem.triggerEventCallback(ActionType::ResizeWindow); 
				ENGINE_TRACE("Handled event: {}", static_cast<int>(RESIZE_WINDOW)); 
				e.handled = true; 
				break;
			default: 
				break;
		}
		
	}
}