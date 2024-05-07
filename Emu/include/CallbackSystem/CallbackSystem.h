#pragma once

#include "../Scenes/SceneObject.h"
#include "CallbackType.h"

#include <variant>
#include <unordered_map>
#include <functional>
#include <string>

// This is the callback system interface. It is a singleton that allows anyone to create callbacks and trigger them.
// There are built in types for registering and triggering callbacks. The types are defined in CallbackType.h.

namespace Engine
{
    using Data = std::variant<std::monostate, const int, const float, const std::string, const std::pair<int, int>, SceneObject*>;
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
        EMU_API void TriggerCallback(Type eventType, Data eventData);
    };
}