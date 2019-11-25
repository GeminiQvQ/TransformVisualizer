#pragma once

// Project Includes
#include <GXGfx/API/Texture.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	class D3D11Texture : public Texture
	{
	public:
		// Construction & Destruction
						D3D11Texture	(const TextureParameters& parameters, D3D11Device& device);
						~D3D11Texture	();

		// Texture Interface
		virtual void	bind			(DeviceContext& device_context) override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}