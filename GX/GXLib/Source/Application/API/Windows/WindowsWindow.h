#pragma once

// Project Includes
#include <GXLib/Application/Window.h>
#include <GXLib/API.h>

// Stdlib Includes
#include <memory>

// Third-Party Includes
#include <windows.h>

namespace GX
{
	class GXLIBS_API WindowsWindow : public Window
	{
	public:
		// Construction & Destruction
								WindowsWindow	(const WindowParameters& parameters, HINSTANCE instance);
								~WindowsWindow	();

		// Properties
		virtual	WindowHandle	handle			() const override;

		virtual void			set_width		(int width) override;
		virtual int				width			() const override;

		virtual void			set_height		(int height) override;
		virtual int				height			() const override;

		virtual void			set_visible		(bool visible) override;
		virtual bool			is_visible		() const override;

		// Operations
		virtual void			repaint			() override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}