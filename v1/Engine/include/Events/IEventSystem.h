#pragma once

#include "../Actions/ActionTypes.h"

#include <variant>
#include <unordered_map>
#include <functional>
#include <string>

using EventData = std::variant<std::monostate, int, float, std::string, std::pair<int, int>>;
using ActionCallback = std::function<void(EventData)>;

namespace Engine
{
    class IEventSystem
    { 
    private:
        std::unordered_map<ActionType, ActionCallback> m_eventCallbacks;
    public:
        void newEventCallback(ActionType actionType, ActionCallback callback);
        void triggerEventCallback(ActionType actionType, EventData eventData);
    };
}
