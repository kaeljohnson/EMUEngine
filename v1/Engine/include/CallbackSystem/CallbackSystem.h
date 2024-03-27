#pragma once

#include "../GameObjects/GameObject.h"
#include "CallbackType.h"

#include <variant>
#include <unordered_map>
#include <functional>
#include <string>

namespace Engine
{
    using Data = std::variant<std::monostate, int, float, std::string, std::pair<int, int>, GameObject*>;
    using Callback = std::function<void(Data)>;

    class ICallbackSystem
    {
    private:
        std::unordered_map<Type, Callback> m_callbacks;
    public:
        void newCallback(Type callbackType, Callback callback);
        void triggerCallback(Type layerEventType, Data eventData);
    };
}