#pragma once

// Project Includes
#include <GXGfx/API/BlendState.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
struct ID3D11BlendState;

namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	class D3D11BlendState : public BlendState
	{
	public:
		// Construction & Destruction
		D3D11BlendState(const BlendStateParameters& parameters, D3D11Device& device);
		~D3D11BlendState();

		// Accessors
		ID3D11BlendState& d3d11_blend_state();

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}