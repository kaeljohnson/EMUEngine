#pragma once

#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
    void ICallbackSystem::newCallback(Type callbackType, Callback callback)
    {
        m_callbacks[callbackType] = callback;
    }

    void ICallbackSystem::triggerCallback(Type callbackType, Data eventData)
    {
        m_callbacks[callbackType](eventData);
    }
}