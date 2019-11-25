#pragma once

// Project Includes
#include <GXGfx/API/D3D11/D3D11RenderTarget.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	class GXGFX_API D3D11RenderTargetBackBuffer : public D3D11RenderTarget
	{
	public:
		// Construction & Destruction
										D3D11RenderTargetBackBuffer		(D3D11Device& device);
										~D3D11RenderTargetBackBuffer	();

		// Accessors
		virtual ID3D11RenderTargetView&	render_target_view				() const override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}