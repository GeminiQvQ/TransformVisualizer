#pragma once

// Forward Declarations
namespace GX::Gfx
{
	class DeviceContext;
}

namespace GX::Gfx
{
	struct TextureParameters
	{
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