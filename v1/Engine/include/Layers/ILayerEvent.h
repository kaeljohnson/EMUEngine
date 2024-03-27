#pragma once

#include "../Layers/LayerEvents.h"
#include "../GameObjects/GameObject.h"

#include <variant>
#include <unordered_map>
#include <functional>
#include <string>

namespace Engine
{
    using LayerEventData = std::variant<std::monostate, int, float, std::string, std::pair<int, int>, GameObject*>;
    using Callback = std::function<void(LayerEventData)>;

    class ILayerEvent
    {
    private:
        std::unordered_map<LayerEvent, Callback> m_layerEventCallbacks;
    public:
        void newLayerEventCallback(LayerEvent layerEventType, Callback callback);
        void triggerLayerEventCallback(LayerEvent layerEventType, LayerEventData eventData);
    };
}