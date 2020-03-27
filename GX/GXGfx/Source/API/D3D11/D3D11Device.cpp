// Local Includes
#include <GXGfx/API/D3D11/D3D11Device.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>
#include <GXGfx/API/D3D11/D3D11RenderTargetBackBuffer.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11Device::Internal
	{
		ID3D11Device*		 d3d11_device;
		ID3D11DeviceContext* d3d11_device_context;
		IDXGISwapChain*		 d3d11_swap_chain;
		
		Internal()
			: d3d11_device {}
			, d3d11_device_context {}
			, d3d11_swap_chain {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11Device::D3D11Device(HWND hwnd)
		: m {std::make_unique<Internal>()}
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.Width = 0;
		swap_chain_desc.BufferDesc.Height = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = hwnd;
		swap_chain_desc.SampleDesc.Count = 4;
		swap_chain_desc.Windowed = true;
		
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			0, //D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swap_chain_desc,
			&m->d3d11_swap_chain,
			&m->d3d11_device,
			nullptr,
			&m->d3d11_device_context
		);
	}

	D3D11Device::~D3D11Device()
	{
		m->d3d11_device->Release();
		m->d3d11_device_context->Release();
		m->d3d11_swap_chain->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Device Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11Device::present()
	{
		m->d3d11_swap_chain->Present(1, 0);
	}

	void D3D11Device::resize_backbuffer(int width, int height)
	{
		m->d3d11_swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11Device& D3D11Device::d3d11_device()
	{
		return *m->d3d11_device;
	}

	ID3D11DeviceContext& D3D11Device::d3d11_device_context()
	{
		return *m->d3d11_device_context;
	}

	IDXGISwapChain& D3D11Device::d3d11_swap_chain()
	{
		return *m->d3d11_swap_chain;
	}
}