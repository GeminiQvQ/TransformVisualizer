// Local Includes
#include <GXLib/Platform/Application.h>

// Stdlib Includes
#include <cassert>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Events
	//-----------------------------------------------------------------------------------------------------

	void Application::add_event_handler(EventHandler& handler)
	{
		auto it = std::find(m_event_handlers.begin(), m_event_handlers.end(), &handler);
		assert(it == m_event_handlers.end());		
		m_event_handlers.push_back(&handler);
	}

	void Application::remove_event_handler(EventHandler& handler)
	{
		auto it = std::find(m_event_handlers.begin(), m_event_handlers.end(), &handler);
		assert(it != m_event_handlers.end());
		m_event_handlers.erase(it);
	}
}