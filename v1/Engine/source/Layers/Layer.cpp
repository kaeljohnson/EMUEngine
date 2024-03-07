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

	void Layer::onAttach() { ENGINE_INFO("Inside layer {} attach function.", m_name); };
	void Layer::onDetach() { ENGINE_INFO("Inside layer {} detach function.", m_name); };
	void Layer::free() { ENGINE_INFO("Inside layer {} free function.", m_name); };
	void Layer::onUpdate() { ENGINE_INFO("Update layer {}", m_name); };
	void Layer::processEvent(Event& e) { ENGINE_INFO("Process event for layer {}", m_name); };

	const std::string& Layer::getName() const { return m_name; }
}