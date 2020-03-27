// Local Includes
#include <GXGfx/API/D3D11/D3D11SamplerState.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11SamplerState::Internal
	{
		ID3D11SamplerState* d3d11_ampler_state;

		Internal(ID3D11SamplerState* d3d11_ampler_state)
			: d3d11_ampler_state {d3d11_ampler_state}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11SamplerState::D3D11SamplerState(ID3D11SamplerState* d3d11_sampler_state)
		: m {std::make_unique<Internal>(d3d11_sampler_state)}
	{
	}

	D3D11SamplerState::~D3D11SamplerState()
	{
		m->d3d11_ampler_state->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11SamplerState& D3D11SamplerState::d3d11_sampler_state()
	{
		return *m->d3d11_ampler_state;
	}

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11SamplerState> create_sampler_state(const SamplerStateParameters& /*parameters*/, D3D11Device& device)
	{
		ID3D11SamplerState* d3d11_sampler_state = nullptr;

		D3D11_SAMPLER_DESC desc {};
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 4;
		desc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
		desc.MinLOD = 0.0f;
		desc.MaxLOD = 0.0f;

		const auto result = device.d3d11_device().CreateSamplerState(&desc, &d3d11_sampler_state);
		if (FAILED(result))
		{
			return nullptr;
		}

		return std::make_unique<D3D11SamplerState>(d3d11_sampler_state);
	}
}