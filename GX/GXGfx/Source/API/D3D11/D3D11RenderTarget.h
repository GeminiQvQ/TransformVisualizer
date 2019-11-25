#pragma once

// Project Includes
#include <GXGfx/API/RenderTarget.h>

// Forward Declarations
struct ID3D11RenderTargetView;

namespace GX::Gfx
{
	class D3D11RenderTarget : public RenderTarget
	{
	public:
		// Accessors
		virtual ID3D11RenderTargetView& render_target_view() const = 0;
	};
}