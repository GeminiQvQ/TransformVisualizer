// Local Includes
#include <GXGfx/API/D3D11/D3D11RenderTargetBackBuffer.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11RenderTargetBackBuffer::Internal
	{
		ID3D11RenderTargetView* render_target_view;

		Internal()
			: render_target_view {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11RenderTargetBackBuffer::D3D11RenderTargetBackBuffer(D3D11Device& device)
		: m {std::make_unique<Internal>()}
	{
		ID3D11Texture2D* texture {};
		device.d3d11_swap_chain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&texture));

		device.d3d11_device().CreateRenderTargetView(texture, nullptr, &m->render_target_view);
		
		texture->Release();
	}

	D3D11RenderTargetBackBuffer::~D3D11RenderTargetBackBuffer()
	{
		m->render_target_view->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11RenderTargetView& D3D11RenderTargetBackBuffer::render_target_view() const
	{
		return *m->render_target_view;
	}
}