#pragma once

// Project Includes
#include <GXGfx/API/SamplerState.h>
#include <GXGfx/API.h>

// Forward Declarations
struct ID3D11SamplerState;

namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// D3D11SamplerState
	//-----------------------------------------------------------------------------------------------------

	class D3D11SamplerState : public SamplerState
	{
	public:
		// Construction & Destruction
							D3D11SamplerState	(ID3D11SamplerState* d3d11_sampler_state);
							~D3D11SamplerState	();

		// Accessors
		ID3D11SamplerState& d3d11_sampler_state	();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11SamplerState> create_sampler_state(const SamplerStateParameters& parameters, D3D11Device& device);
}