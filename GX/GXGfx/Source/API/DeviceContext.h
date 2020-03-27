#pragma once

// Forward Declarations
namespace GX::Gfx
{
	class BlendState;
	class ConstantBuffer;
	class PixelShader;
	class RenderTarget;
	class SamplerState;
	class VertexShader;
	struct Viewport;
}

namespace GX::Gfx
{
	class DeviceContext
	{
	public:
		// Construction & Destruction
		virtual				~DeviceContext			() = default;

		// Device Context Interface
		virtual void		clear					(float r, float g, float b, float a) = 0;
		virtual void		set_viewport			(const Viewport& viewport) = 0;
		virtual void		set_render_target		(RenderTarget* render_target) = 0;
		virtual void		set_blend_state			(BlendState* blend_state) = 0;
		virtual void		set_vertex_shader		(VertexShader& shader) = 0;
		virtual void		set_pixel_shader		(PixelShader& shader) = 0;
		virtual void		set_constant_buffer_vs	(ConstantBuffer& buffer) = 0;
		virtual void		set_constant_buffer_ps	(ConstantBuffer& buffer) = 0;
		virtual void		set_sampler_state_vs	(SamplerState& sampler_state) = 0;
		virtual void		set_sampler_state_ps	(SamplerState& sampler_state) = 0;

		virtual Viewport	viewport				() const = 0;
	};
}