#pragma once

#include "../GameObjects/GameObject.h"
#include "CallbackType.h"

#include <variant>
#include <unordered_map>
#include <functional>
#include <string>

namespace Engine
{
    using Data = std::variant<std::monostate, const int, const float, const std::string, const std::pair<int, int>, GameObject*>;
    using Callback = std::function<void(Data)>;

    class EMU_API ICallbackSystem
    {
    private:
		static ICallbackSystem* instance;
		ICallbackSystem() = default;

        std::unordered_map<Type, Callback> m_callbacks;
    public:
        static ICallbackSystem* GetInstance();
        void NewCallback(Type callbackType, Callback callback);
        void TriggerCallback(Type layerEventType, Data eventData);
    };
}