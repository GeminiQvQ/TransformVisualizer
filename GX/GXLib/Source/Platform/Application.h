#pragma once

// Project Includes
#include <GXLib/API.h>

// Stdlib Includes
#include <utility>
#include <vector>

// Forward Declarations
namespace GX
{
	class EventHandler;
	class Window;
	struct WindowParameters;
}

namespace GX
{
	class GXLIBS_API Application
	{
	public:
		virtual							~Application			() = default;

		virtual std::pair<bool, int>	process					() = 0;
		virtual Window*					create_window			(const WindowParameters& parameters) = 0;

		// Events
		void							add_event_handler		(EventHandler& handler);
		void							remove_event_handler	(EventHandler& handler);

	protected:
		// Events
		template <typename Callback, typename... Args>
		void							event_callback			(Callback callback, Args&&... args);

	private:
		std::vector<EventHandler*> m_event_handlers;
	};
}

// Inline Includes
#include <GXLib/Platform/Application.inl>
