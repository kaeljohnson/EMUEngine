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

    class ICallbackSystem
    {
    private:
		static ICallbackSystem* instance;
		ICallbackSystem() = default;

        std::unordered_map<Type, Callback> m_callbacks;
    public:
        EMU_API static ICallbackSystem* GetInstance();
        EMU_API void NewCallback(Type callbackType, Callback callback);
        EMU_API void TriggerCallback(Type layerEventType, Data eventData);
    };
}