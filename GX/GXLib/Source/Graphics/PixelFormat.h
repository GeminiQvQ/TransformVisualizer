#pragma once

// Project Includes
#include <GXLib/API.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	enum class PixelFormat
	{
		R32G32Float,
		R32G32B32Float,
		R32G32B32A32Float,
		R8UNorm,
		R8G8B8A8UNorm,
		R8G8B8A8UNormSRGB,
		B8G8R8A8Unorm,
		R32SInt,

		Invalid
	};

	//-----------------------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------------------

	GXLIBS_API int get_pixel_format_bpp(PixelFormat format);
}