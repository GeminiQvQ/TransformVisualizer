#pragma once

// Project Includes
#include <GXLib/Application/Application.h>

// Stdlib Includes
#include <memory>

// Third-Party Includes
#include <windows.h>

namespace GX
{
	class GXLIBS_API WindowsApplication : public Application
	{
	public:
		// Construction & Destruction
										WindowsApplication	(HINSTANCE instance);
										~WindowsApplication	();


		// Application Interface
		virtual std::pair<bool, int>	process				() override;
		virtual Window*					create_window		(const WindowParameters& parameters) override;

	private:
		// Windows Callbacks
		static std::pair<bool, int>		app_proc			();
		static LRESULT CALLBACK			wnd_proc			(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);

		struct Internal;
		const std::unique_ptr<Internal> m;
	};	
}