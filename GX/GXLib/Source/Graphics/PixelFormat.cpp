// Local Includes
#include <GXLib/Graphics/PixelFormat.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------------------

	int get_pixel_format_bpp(PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::R32G32B32A32Float:
			return 128;
		case PixelFormat::R32G32B32Float:
			return 96;
		case PixelFormat::R32G32Float:
			return 64;
		case PixelFormat::R8G8B8A8UNorm:
		case PixelFormat::R8G8B8A8UNormSRGB:
		case PixelFormat::B8G8R8A8Unorm:
		case PixelFormat::R32SInt:
			return 32;
		case PixelFormat::R8UNorm:
			return 8;
		default:
			return 0;
		}
	}
}