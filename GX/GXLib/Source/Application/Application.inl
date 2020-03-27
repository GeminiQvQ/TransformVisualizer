// Stdlib Includes
#include <functional>

namespace GX
{
	template <typename Callback, typename... Args>
	void Application::event_callback(Callback callback, Args&&... args)
	{
		for (auto event_handler : m_event_handlers)
		{
			std::invoke(callback, event_handler, std::forward<Args>(args)...);
		}
	}
}