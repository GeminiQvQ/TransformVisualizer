#pragma once

// Project Includes
#include <GXLib/Application/Types.h>
#include <GXLib/API.h>

// Stdlib Includes
#include <vector>

// Forward Declarations
namespace GX
{
	class Window;
}

namespace GX
{
	class GXLIBS_API EventHandler
	{
	public:
		// Window
		virtual void on_window_create		(Window& window);
		virtual void on_window_destroy		(Window& window);
		virtual void on_window_resize		(Window& window, int width, int height);

		// Input
		virtual void on_key_down			(Window* window, Key key, const std::vector<Modifier>& modifiers);
		virtual void on_key_up				(Window* window, Key key, const std::vector<Modifier>& modifiers);
		virtual void on_mouse_button_down	(Window* window, MouseButton button, int x, int y, const std::vector<Modifier>& modifiers);
		virtual void on_mouse_button_up		(Window* window, MouseButton button, int x, int y, const std::vector<Modifier>& modifiers);
		virtual void on_mouse_move			(Window* window, int x, int y);
	};
}