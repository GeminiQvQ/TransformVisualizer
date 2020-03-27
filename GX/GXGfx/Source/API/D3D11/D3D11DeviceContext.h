#pragma once

// Project Includes
#include <GXGfx/API/DeviceContext.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
struct ID3D11DeviceContext;

namespace GX::Gfx
{
	class D3D11DeviceContext : public DeviceContext
	{
	public:
		// Construction & Destruction
								D3D11DeviceContext		(ID3D11DeviceContext& d3d11_device_context);
								~D3D11DeviceContext		();

		// Device Context Interface
		virtual void			clear					(float r, float g, float b, float a) override;
		virtual void			set_viewport			(const Viewport& viewport) override;
		virtual void			set_render_target		(RenderTarget* render_target) override;
		virtual void			set_blend_state			(BlendState* blend_state) override;
		virtual void			set_vertex_shader		(VertexShader& shader) override;
		virtual void			set_pixel_shader		(PixelShader& shader) override;
		virtual void			set_constant_buffer_vs	(ConstantBuffer& buffer) override;
		virtual void			set_constant_buffer_ps	(ConstantBuffer& buffer) override;
		virtual void			set_sampler_state_vs	(SamplerState& sampler_state) override;
		virtual void			set_sampler_state_ps	(SamplerState& sampler_state) override;

		virtual Viewport		viewport				() const override;

		// Accessors
		ID3D11DeviceContext&	d3d11_device_context	();

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}