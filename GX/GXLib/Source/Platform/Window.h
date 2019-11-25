#pragma once

// Stdlib Includes
#include <string>
#include <vector>

// Forward Declarations
namespace GX
{
	class WindowEventHandler;
}

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Window Types
	//-----------------------------------------------------------------------------------------------------

	using WindowHandle = size_t;

	struct WindowParameters
	{
		std::wstring		name;
		std::pair<int, int> pos;
		std::pair<int, int> size;
	};

	//-----------------------------------------------------------------------------------------------------
	// Window
	//-----------------------------------------------------------------------------------------------------

	class Window
	{
	public:
		// Construction & Destruction
		virtual					~Window			() = default;

		// Properties
		virtual	WindowHandle	handle			() const = 0;

		virtual void			set_width		(int width) = 0;
		virtual int				width			() const = 0;

		virtual void			set_height		(int height) = 0;
		virtual int				height			() const = 0;

		virtual void			set_visible		(bool visible) = 0;
		virtual bool			is_visible		() const = 0;

		// Operations
		virtual void			repaint			() = 0;

	protected:
		// Events
		template <typename Callback, typename... Args>
		void					event_callback	(Callback callback, Args... args);

	private:
		std::vector<WindowEventHandler*> m_event_handlers;
	};
}