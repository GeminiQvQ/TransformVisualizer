// Local Includes
#include <GXLib/Application/API/Windows/WindowsApplication.h>

// Project Includes
#include <GXLib/Application/API/Windows/WindowsTypes.h>
#include <GXLib/Application/API/Windows/WindowsWindow.h>
#include <GXLib/Application/EventHandler.h>

// Stdlib Includes
#include <cassert>
#include <map>
#include <vector>

// Third-Party Includes
#include <windows.h>
#include <windowsx.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct WindowsApplication::Internal
	{
		static WindowsApplication*	s_instance;

		HINSTANCE					instance;
		std::map<HWND, Window*>		windows;

		Internal(HINSTANCE instance)
			: instance {instance}
		{
		}
	};

	WindowsApplication* WindowsApplication::Internal::s_instance {nullptr};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	WindowsApplication::WindowsApplication(HINSTANCE instance)
		: m {std::make_unique<Internal>(instance)}
	{
		// Singleton.
		assert(Internal::s_instance == nullptr);
		Internal::s_instance = this;

		// Window class.
		WNDCLASS window_class {};
		window_class.lpfnWndProc = &WindowsApplication::wnd_proc;
		window_class.hInstance = instance;
		window_class.lpszClassName = L"Window";
		window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		RegisterClass(&window_class);
	}

	WindowsApplication::~WindowsApplication()
	{
		Internal::s_instance = nullptr;
	}

	//-----------------------------------------------------------------------------------------------------
	// Application Interface
	//-----------------------------------------------------------------------------------------------------

	std::pair<bool, int> WindowsApplication::process()
	{
		return app_proc();
	}

	Window* WindowsApplication::create_window(const WindowParameters& parameters)
	{
		auto window = new WindowsWindow(parameters, m->instance);
		m->windows.insert(std::make_pair(reinterpret_cast<HWND>(window->handle()), window));
		event_callback(&EventHandler::on_window_create, *window);
		return window;
	}

	//-----------------------------------------------------------------------------------------------------
	// Windows Callbacks
	//-----------------------------------------------------------------------------------------------------

	std::pair<bool, int> WindowsApplication::app_proc()
	{
		MSG message {};
		auto message_result = GetMessage(&message, 0, 0, 0);

		std::pair<bool, int> result {std::make_pair(true, (int)message.wParam)};

		if (message_result != 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			result.first = false;
		}

		return result;
	}

	LRESULT WindowsApplication::wnd_proc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		auto app {Internal::s_instance};

		switch (message)
		{
		case WM_CLOSE:
			{
				DestroyWindow(window_handle);
			}
			break;

		case WM_DESTROY:
			{
				auto it = app->m->windows.find(window_handle);
				assert(it != app->m->windows.end());
				app->event_callback(&EventHandler::on_window_destroy, *it->second);
				app->m->windows.erase(it);

				if (app->m->windows.empty())
				{
					PostQuitMessage(0);
				}
			}
			break;

		case WM_SIZE:
			{
				auto it = app->m->windows.find(window_handle);
				assert(it != app->m->windows.end());
				auto width = LOWORD(lparam);
				auto height = HIWORD(lparam);
				app->event_callback(&EventHandler::on_window_resize, *it->second, width, height);
			}
			break;

		case WM_KEYDOWN:
			{
				auto window_it = app->m->windows.find(window_handle);
				auto window = (window_it != app->m->windows.end() ? window_it->second : nullptr);
				auto key = get_platform_key((int)wparam);
				
				if (key != Key::Invalid)
				{
					app->event_callback(&EventHandler::on_key_down, window, key, std::vector<Modifier>{});
				}
			}
			break;

		case WM_KEYUP:
			{
				auto window_it = app->m->windows.find(window_handle);
				auto window = (window_it != app->m->windows.end() ? window_it->second : nullptr);
				auto key = get_platform_key((int)wparam);
				
				if (key != Key::Invalid)
				{
					app->event_callback(&EventHandler::on_key_up, window, key, std::vector<Modifier>{});
				}
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				auto window_it = app->m->windows.find(window_handle);
				auto window = (window_it != app->m->windows.end() ? window_it->second : nullptr);
				auto x = GET_X_LPARAM(lparam);
				auto y = GET_Y_LPARAM(lparam);
				
				MouseButton button = MouseButton::Invalid;
				switch (message)
				{
				case WM_LBUTTONDOWN: button = MouseButton::Left; break;
				case WM_MBUTTONDOWN: button = MouseButton::Middle; break;
				case WM_RBUTTONDOWN: button = MouseButton::Right; break;
				}

				if (button != MouseButton::Invalid)
				{
					app->event_callback(&EventHandler::on_mouse_button_down, window, button, x, y, std::vector<Modifier>{});
				}
			}
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			{
				auto window_it = app->m->windows.find(window_handle);
				auto window = (window_it != app->m->windows.end() ? window_it->second : nullptr);
				auto x = GET_X_LPARAM(lparam);
				auto y = GET_Y_LPARAM(lparam);

				MouseButton button = MouseButton::Invalid;
				switch (message)
				{
				case WM_LBUTTONUP: button = MouseButton::Left; break;
				case WM_MBUTTONUP: button = MouseButton::Middle; break;
				case WM_RBUTTONUP: button = MouseButton::Right; break;
				}

				if (button != MouseButton::Invalid)
				{
					app->event_callback(&EventHandler::on_mouse_button_up, window, button, x, y, std::vector<Modifier>{});
				}
			}
			break;

		case WM_MOUSEMOVE:
			{
				auto window_it = app->m->windows.find(window_handle);
				auto window = (window_it != app->m->windows.end() ? window_it->second : nullptr);
				auto x = GET_X_LPARAM(lparam);
				auto y = GET_Y_LPARAM(lparam);
				
				app->event_callback(&EventHandler::on_mouse_move, window, x, y);
			}
			break;
		}

		return DefWindowProc(window_handle, message, wparam, lparam);
	}
}