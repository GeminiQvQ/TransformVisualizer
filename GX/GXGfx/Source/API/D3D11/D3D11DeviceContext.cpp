// Local Includes
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11BlendState.h>
#include <GXGfx/API/D3D11/D3D11ConstantBuffer.h>
#include <GXGfx/API/D3D11/D3D11RenderTarget.h>
#include <GXGfx/API/D3D11/D3D11SamplerState.h>
#include <GXGfx/API/D3D11/D3D11Shader.h>
#include <GXGfx/API/Viewport.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11DeviceContext::Internal
	{
		ID3D11DeviceContext& d3d11_device_context;
		D3D11RenderTarget*	 render_target;

		Viewport			 viewport;

		Internal(ID3D11DeviceContext& d3d11_device_context)
			: d3d11_device_context {d3d11_device_context}
			, render_target {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11DeviceContext::D3D11DeviceContext(ID3D11DeviceContext& d3d11_device_context)
		: m {std::make_unique<Internal>(d3d11_device_context)}
	{
	}

	D3D11DeviceContext::~D3D11DeviceContext()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Device Context Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11DeviceContext::clear(float r, float g, float b, float a)
	{
		float rgba[4] {r, g, b, a};
		m->d3d11_device_context.ClearRenderTargetView(&m->render_target->render_target_view(), rgba);
	}

	void D3D11DeviceContext::set_viewport(const Viewport& viewport)
	{
		D3D11_VIEWPORT d3d11_viewport {};
		d3d11_viewport.TopLeftX = viewport.x;
		d3d11_viewport.TopLeftY = viewport.y;
		d3d11_viewport.Width = viewport.width;
		d3d11_viewport.Height = viewport.height;
		m->d3d11_device_context.RSSetViewports(1, &d3d11_viewport);
		m->viewport = viewport;
	}

	void D3D11DeviceContext::set_render_target(RenderTarget* render_target)
	{
		m->render_target = static_cast<D3D11RenderTarget*>(render_target);
		auto render_target_view = (render_target != nullptr ? &m->render_target->render_target_view() : nullptr);
		m->d3d11_device_context.OMSetRenderTargets(1, &render_target_view, nullptr);
	}

	void D3D11DeviceContext::set_blend_state(BlendState* blend_state)
	{
		auto d3d11_blend_state = (blend_state != nullptr ? &static_cast<D3D11BlendState*>(blend_state)->d3d11_blend_state() : nullptr);
		m->d3d11_device_context.OMSetBlendState(d3d11_blend_state, nullptr, 0xFFFFFFFF);
	}

	void D3D11DeviceContext::set_vertex_shader(VertexShader& shader)
	{
		m->d3d11_device_context.VSSetShader(&static_cast<D3D11VertexShader&>(shader).d3d11_shader(), nullptr, 0);
	}

	void D3D11DeviceContext::set_pixel_shader(PixelShader& shader)
	{
		m->d3d11_device_context.PSSetShader(&static_cast<D3D11PixelShader&>(shader).d3d11_shader(), nullptr, 0);
	}

	void D3D11DeviceContext::set_constant_buffer_vs(ConstantBuffer& buffer)
	{
		auto d3d11_buffer = &static_cast<D3D11ConstantBuffer&>(buffer).d3d11_buffer();
		m->d3d11_device_context.VSSetConstantBuffers(0, 1, &d3d11_buffer);
	}

	void D3D11DeviceContext::set_constant_buffer_ps(ConstantBuffer& buffer)
	{
		auto d3d11_buffer = &static_cast<D3D11ConstantBuffer&>(buffer).d3d11_buffer();
		m->d3d11_device_context.PSSetConstantBuffers(1, 1, &d3d11_buffer);
	}

	void D3D11DeviceContext::set_sampler_state_vs(SamplerState& sampler_state)
	{
		auto d3d11_sampler_state = &static_cast<D3D11SamplerState&>(sampler_state).d3d11_sampler_state();
		m->d3d11_device_context.VSSetSamplers(0, 1, &d3d11_sampler_state);
	}

	void D3D11DeviceContext::set_sampler_state_ps(SamplerState& sampler_state)
	{
		auto d3d11_sampler_state = &static_cast<D3D11SamplerState&>(sampler_state).d3d11_sampler_state();
		m->d3d11_device_context.PSSetSamplers(0, 1, &d3d11_sampler_state);
	}

	//-----------------------------------------------------------------------------------------------------

	Viewport D3D11DeviceContext::viewport() const
	{
		return m->viewport;
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11DeviceContext& D3D11DeviceContext::d3d11_device_context()
	{
		return m->d3d11_device_context;
	}
}