#pragma once

#include "../../include/Actions/ActionTypes.h"
#include "../../include/Events/IEventSystem.h"

namespace Engine
{
    void IEventSystem::newEventCallback(const ActionType actionType, ActionCallback callback)
    {
        m_eventCallbacks[actionType] = callback;
    }

    void IEventSystem::triggerEventCallback(const ActionType actionType, EventData eventData)
    {
        m_eventCallbacks[actionType](eventData);
    }
}