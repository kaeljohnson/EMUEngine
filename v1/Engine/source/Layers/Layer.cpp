#pragma once

#include <string>

#include "../../include/Layers/Layer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	Layer::Layer(std::string name)
		: m_name(name), m_enabled(true) {}
	

	Layer::~Layer()
	{

	}

	void Layer::onAttach() { ENGINE_TRACE("Inside layer {} attach function.", m_name); };
	void Layer::onDetach() { ENGINE_TRACE("Inside layer {} detach function.", m_name); };
	void Layer::onUpdate() { ENGINE_TRACE("Update layer {}", m_name); };
	void Layer::processEvent(Event& e) { ENGINE_TRACE("Process event for layer {}", m_name); };

	const std::string& Layer::getName() const { return m_name; }
}