#pragma once

// Project Includes
#include <GXGfx/API/Device.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <memory>

// Third-Party Includes
#include <windows.h>

// Forward Declarations
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace GX
{
	class Window;
}

namespace GX::Gfx
{
	class GXGFX_API D3D11Device : public Device
	{
	public:
		// Construction & Destruction
								D3D11Device				(HWND hwnd);
								~D3D11Device			();

		// Device Interface
		virtual void			present					() override;
		virtual void			resize_backbuffer		(int width, int height) override;

		// Accessors
		ID3D11Device&			d3d11_device			();
		ID3D11DeviceContext&	d3d11_device_context	();
		IDXGISwapChain&			d3d11_swap_chain		();

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}