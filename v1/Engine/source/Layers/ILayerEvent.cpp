#pragma once

#include "../../include/Layers/LayerEvents.h"
#include "../../include/Layers/ILayerEvent.h"

namespace Engine
{
    void ILayerEvent::newLayerEventCallback(LayerEvent layerEventType, Callback callback)
    {
		m_layerEventCallbacks[layerEventType] = callback;
	}

    void ILayerEvent::triggerLayerEventCallback(LayerEvent layerEventType, LayerEventData eventData)
    {
        m_layerEventCallbacks[layerEventType](eventData);
    }
}