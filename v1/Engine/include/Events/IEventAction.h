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
    class IEventAction
    { 
    private:
        std::unordered_map<ActionType, ActionCallback> m_actionCallbacks;
    public:
        void newActionCallback(ActionType actionType, ActionCallback callback);
        void triggerActionCallback(ActionType actionType, EventData eventData);
    };
}
