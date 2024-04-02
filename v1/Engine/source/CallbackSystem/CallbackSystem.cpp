#pragma once

#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{

    ICallbackSystem* ICallbackSystem::getInstance()
    {
        if (instance == nullptr)
        {
			instance = new ICallbackSystem();
		}

		return instance;
    }

    void ICallbackSystem::newCallback(Type callbackType, Callback callback)
    {
        if (instance == nullptr)
        {
            ENGINE_CRITICAL("Callback system not initialized!");
            return;
        }

        m_callbacks[callbackType] = callback;
    }

    void ICallbackSystem::triggerCallback(Type callbackType, Data eventData)
    {
        if (instance == nullptr)
        {
			ENGINE_CRITICAL("Callback system not initialized!");
			return;
		}

        m_callbacks[callbackType](eventData);
    }
}