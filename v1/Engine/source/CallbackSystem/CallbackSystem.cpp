#pragma once

#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
    ICallbackSystem* ICallbackSystem::GetInstance()
    {
        if (instance == nullptr)
        {
			instance = new ICallbackSystem();
		}

		return instance;
    }

    void ICallbackSystem::NewCallback(Type callbackType, Callback callback)
    {
        if (instance == nullptr)
        {
            ENGINE_CRITICAL_D("Callback system not initialized!");
            return;
        }

        m_callbacks[callbackType] = callback;
    }

    void ICallbackSystem::TriggerCallback(Type callbackType, Data eventData)
    {
        if (instance == nullptr)
        {
			ENGINE_CRITICAL_D("Callback system not initialized!");
			return;
		}

        m_callbacks[callbackType](eventData);
    }
}