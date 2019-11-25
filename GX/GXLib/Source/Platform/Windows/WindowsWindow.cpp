// Local Includes
#include <GXLib/Platform/Windows/WindowsWindow.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct WindowsWindow::Internal
	{
		HWND hwnd;

		Internal()
			: hwnd {0}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	WindowsWindow::WindowsWindow(const WindowParameters& parameters, HINSTANCE instance)
		: m {std::make_unique<Internal>()}
	{
		m->hwnd = CreateWindowEx(
			0,
			L"Window",
			parameters.name.c_str(),
			WS_OVERLAPPEDWINDOW,
			parameters.pos.first,
			parameters.pos.second,
			parameters.size.first,
			parameters.size.second,
			0,
			0,
			instance,
			0
		);
	}

	WindowsWindow::~WindowsWindow()
	{
		CloseWindow(m->hwnd);
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	WindowHandle WindowsWindow::handle() const
	{
		return reinterpret_cast<WindowHandle>(m->hwnd);
	}

	//-----------------------------------------------------------------------------------------------------

	void WindowsWindow::set_width(int width)
	{
		RECT rect {};
		GetWindowRect(m->hwnd, &rect);
		rect.left = rect.right + width;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	}

	int WindowsWindow::width() const
	{
		RECT rect {};
		GetWindowRect(m->hwnd, &rect);
		return (rect.right - rect.left);
	}

	//-----------------------------------------------------------------------------------------------------

	void WindowsWindow::set_height(int height)
	{
		RECT rect {};
		GetWindowRect(m->hwnd, &rect);
		rect.bottom = rect.top + height;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	}

	int WindowsWindow::height() const
	{
		RECT rect {};
		GetWindowRect(m->hwnd, &rect);
		return (rect.bottom - rect.top);
	}

	//-----------------------------------------------------------------------------------------------------

	void WindowsWindow::set_visible(bool visible)
	{
		ShowWindow(m->hwnd, visible ? SW_SHOW : SW_HIDE);
	}

	bool WindowsWindow::is_visible() const
	{
		WINDOWPLACEMENT placement {};
		GetWindowPlacement(m->hwnd, &placement);
		return (placement.showCmd == SW_SHOW);
	}

	//-----------------------------------------------------------------------------------------------------
	// Operations
	//-----------------------------------------------------------------------------------------------------

	void WindowsWindow::repaint()
	{
		InvalidateRect(m->hwnd, 0, true);
	}
}