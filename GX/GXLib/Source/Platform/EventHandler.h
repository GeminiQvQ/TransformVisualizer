#pragma once

// Project Includes
#include <GXLib/Platform/PlatformTypes.h>

// Stdlib Includes
#include <vector>

namespace GX
{
	class Window;
}

namespace GX
{
	class EventHandler
	{
	public:
		// Window
		virtual void on_window_create		(Window& window);
		virtual void on_window_destroy		(Window& window);
		virtual void on_window_resize		(Window& window, int width, int height);

		// Input
		virtual void on_key_down			(Window* window, Key key, const std::vector<Modifier>& modifiers);
		virtual void on_key_up				(Window* window, Key key, const std::vector<Modifier>& modifiers);
		virtual void on_mouse_button_down	(Window* window, MouseButton button, const std::vector<Modifier>& modifiers);
		virtual void on_mouse_button_up		(Window* window, MouseButton button, const std::vector<Modifier>& modifiers);
	};
}