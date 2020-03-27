#pragma once

// Project Includes
#include <GXLib/Graphics/PixelFormat.h>

// Forward Declarations
namespace GX::Gfx
{
	class DeviceContext;
}

namespace GX::Gfx
{
	struct TextureParameters
	{
		size_t		width;
		size_t		height;
		PixelFormat format;
		const void* data;
	};

	class Texture
	{
	public:
		// Construction & Destruction
		virtual			~Texture	() = default;

		// Texture Interface
		virtual void	bind		(DeviceContext& device_context) = 0;
	};
}