#pragma once

#include "../Actions/ActionTypes.h"

#include <functional>

namespace Engine
{
    class IEventSystem
    {
    private:
        using EventCallback = std::function<void()>;
    public:
        virtual void newEventCallback(ActionType actionType, EventCallback callback) = 0;
        virtual void triggerEventCallback(ActionType actionType) = 0;
    };
}
