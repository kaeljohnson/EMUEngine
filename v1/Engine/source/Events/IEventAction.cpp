#pragma once

#include "../../include/Actions/ActionTypes.h"
#include "../../include/Events/IEventAction.h"

namespace Engine
{
    void IEventAction::newActionCallback(const ActionType actionType, ActionCallback callback)
    {
        m_actionCallbacks[actionType] = callback;
    }

    void IEventAction::triggerActionCallback(const ActionType actionType, EventData eventData)
    {
        m_actionCallbacks[actionType](eventData);
    }
}