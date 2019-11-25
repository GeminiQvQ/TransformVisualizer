// Local Includes
#include <GXGfx/API/D3D11/D3D11BlendState.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>

// Stdlib Includes
#include <map>

// Third-Party Includes
#include <d3d11.h>

namespace
{
	//-----------------------------------------------------------------------------------------------------
	// Type Maps
	//-----------------------------------------------------------------------------------------------------

	const std::map<GX::Gfx::BlendType, D3D11_BLEND>& blend_type_map()
	{
		using namespace GX::Gfx;
		static const std::map<BlendType, D3D11_BLEND> s_map
		{
			{ BlendType::SrcColor,	  D3D11_BLEND_SRC_COLOR		},
			{ BlendType::InvSrcColor, D3D11_BLEND_INV_SRC_COLOR },
			{ BlendType::SrcAlpha,	  D3D11_BLEND_SRC_ALPHA		},
			{ BlendType::InvSrcAlpha, D3D11_BLEND_INV_SRC_ALPHA }
		};
		return s_map;
	}

	//-----------------------------------------------------------------------------------------------------

	const std::map<GX::Gfx::BlendOp, D3D11_BLEND_OP>& blend_op_map()
	{
		using namespace GX::Gfx;
		static const std::map<BlendOp, D3D11_BLEND_OP> s_map
		{
			{ BlendOp::Add,	D3D11_BLEND_OP_ADD }
		};
		return s_map;
	}
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11BlendState::Internal
	{
		ID3D11BlendState* d3d11_blend_state;

		Internal()
			: d3d11_blend_state {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11BlendState::D3D11BlendState(const BlendStateParameters& parameters, D3D11Device& device)
		: m {std::make_unique<Internal>()}
	{
		D3D11_BLEND_DESC desc {};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = parameters.enabled;
		desc.RenderTarget[0].SrcBlend = blend_type_map().find(parameters.blend_src)->second;
		desc.RenderTarget[0].DestBlend = blend_type_map().find(parameters.blend_dst)->second;
		desc.RenderTarget[0].BlendOp = blend_op_map().find(parameters.blend_op)->second;
		desc.RenderTarget[0].SrcBlendAlpha = blend_type_map().find(parameters.blend_alpha_src)->second;
		desc.RenderTarget[0].DestBlendAlpha = blend_type_map().find(parameters.blend_alpha_dst)->second;
		desc.RenderTarget[0].BlendOpAlpha = blend_op_map().find(parameters.blend_alpha_op)->second;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		device.d3d11_device().CreateBlendState(&desc, &m->d3d11_blend_state);
	}

	D3D11BlendState::~D3D11BlendState()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11BlendState& D3D11BlendState::d3d11_blend_state()
	{
		return *m->d3d11_blend_state;
	}
}