#pragma once

// Project Includes
#include <GXLib/Graphics/PixelFormat.h>
#include <GXLib/API.h>

// Third-Party Includes
#include <dxgiformat.h>

namespace GX
{
	// DXGI Format Conversion
	GXLIBS_API DXGI_FORMAT to_dxgi_format	(PixelFormat format);
	GXLIBS_API PixelFormat from_dxgi_format	(DXGI_FORMAT format);
}