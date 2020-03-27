// Local Includes
#include <GXGfx/API/D3D11/D3D11Texture.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>
#include <GXLib/Graphics/API/Windows/WindowsPixelFormat.h>

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

		Internal(ID3D11Texture2D* d3d11_texture, ID3D11ShaderResourceView* d3d11_resource_view)
			: d3d11_texture {d3d11_texture}
			, d3d11_resource_view {d3d11_resource_view}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11Texture::D3D11Texture(ID3D11Texture2D* d3d11_texture, ID3D11ShaderResourceView* d3d11_resource_view)
		: m {std::make_unique<Internal>(d3d11_texture, d3d11_resource_view)}
	{
	}

	D3D11Texture::~D3D11Texture()
	{
		m->d3d11_texture->Release();
		m->d3d11_resource_view->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Texture Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11Texture::bind(DeviceContext& device_context)
	{
		auto& d3d11_device_context = static_cast<D3D11DeviceContext&>(device_context).d3d11_device_context();
		d3d11_device_context.PSSetShaderResources(0, 1, &m->d3d11_resource_view);
	}

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11Texture> create_texture(const TextureParameters& parameters, D3D11Device& device)
	{
		// Create texture.
		ID3D11Texture2D* d3d11_texture = nullptr;
		{
			D3D11_TEXTURE2D_DESC desc {};
			desc.Width = (UINT)parameters.width;
			desc.Height = (UINT)parameters.height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.Format = to_dxgi_format(parameters.format);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			const int bytes_per_pixel = get_pixel_format_bpp(parameters.format) / 8;

			D3D11_SUBRESOURCE_DATA data {};
			data.pSysMem = parameters.data;
			data.SysMemPitch = (UINT)(parameters.width * bytes_per_pixel);
			data.SysMemSlicePitch = (UINT)(parameters.width * parameters.height * bytes_per_pixel);

			const auto result = device.d3d11_device().CreateTexture2D(&desc, &data, &d3d11_texture);
			if (FAILED(result))
			{
				return nullptr;
			}
		}

		// Create resource view.
		ID3D11ShaderResourceView* d3d11_resource_view = nullptr;
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc {};
			desc.Format = to_dxgi_format(parameters.format);
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;

			const auto result = device.d3d11_device().CreateShaderResourceView(d3d11_texture, &desc, &d3d11_resource_view);
			if (FAILED(result))
			{
				d3d11_texture->Release();
				return nullptr;
			}
		}

		return std::make_unique<D3D11Texture>(d3d11_texture, d3d11_resource_view);
	}
}