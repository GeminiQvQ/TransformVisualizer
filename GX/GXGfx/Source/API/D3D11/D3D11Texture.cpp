// Local Includes
#include <GXGfx/API/D3D11/D3D11Texture.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11Texture::Internal
	{
		ID3D11Texture2D*		  d3d11_texture;
		ID3D11ShaderResourceView* d3d11_resource_view;

		Internal()
			: d3d11_texture {}
			, d3d11_resource_view {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11Texture::D3D11Texture(const TextureParameters& parameters, D3D11Device& device)
		: m {std::make_unique<Internal>()}
	{
		// TODO
		parameters;
		device;
	}

	D3D11Texture::~D3D11Texture()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Texture Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11Texture::bind(DeviceContext& device_context)
	{
		auto& d3d11_device_context = static_cast<D3D11DeviceContext&>(device_context).d3d11_device_context();
		d3d11_device_context.PSSetShaderResources(0, 1, &m->d3d11_resource_view);
	}
}