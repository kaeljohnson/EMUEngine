#pragma once

#include <string>

#include "../Events/Event.h"
#include "../Events/Event.h"

namespace Engine
{
	class Layer
	{
	protected:
		std::string m_name;
		bool m_enabled;
	public:
		Layer(std::string name);
		virtual void free();
		virtual	~Layer();

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate();
		virtual void processEvent(Event& e);

		const std::string& getName() const;
	};
}

