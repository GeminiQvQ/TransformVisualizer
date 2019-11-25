#pragma once

namespace GX::Gfx
{
	class Device
	{
	public:
		// Construction & Destruction
		virtual			~Device				() = default;

		// Device Interface
		virtual void	present				() = 0;
		virtual void	resize_backbuffer	(int width, int height) = 0;
	};
}