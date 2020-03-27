#pragma once

// Project Includes
#include <GXGfx/API/Texture.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// D3D11Texture
	//-----------------------------------------------------------------------------------------------------

	class D3D11Texture : public Texture
	{
	public:
		// Construction & Destruction
						D3D11Texture	(ID3D11Texture2D* d3d11_texture, ID3D11ShaderResourceView* d3d11_resource_view);
						~D3D11Texture	();

		// Texture Interface
		virtual void	bind			(DeviceContext& device_context) override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11Texture> create_texture(const TextureParameters& parameters, D3D11Device& device);
}