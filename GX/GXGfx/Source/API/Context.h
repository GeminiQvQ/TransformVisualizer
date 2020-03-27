#pragma once

// Project Includes
#include <GXGfx/API/RenderTarget.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <string_view>
#include <vector>

// Forward Declarations
namespace GX::Gfx
{
	class BlendState;
	class ConstantBuffer;
	class Device;
	class DeviceContext;
	class PixelShader;
	class SamplerState;
	class Texture;
	class VertexShader;
	class VertexBuffer;
	struct BlendStateParameters;
	struct ConstantBufferParameters;
	struct SamplerStateParameters;
	struct TextureParameters;
	struct VertexBufferParameters;
}

namespace GX::Gfx
{
	class Context
	{
	public:
		// Construction & Destruction
		virtual							~Context				() = default;

		// Resources
		virtual UPtr<VertexShader>		create_vertex_shader	(std::wstring_view filepath) = 0;
		virtual UPtr<PixelShader>		create_pixel_shader		(std::wstring_view filepath) = 0;
		virtual UPtr<VertexBuffer>		create_vertex_buffer	(const VertexBufferParameters& parameters) = 0;
		virtual UPtr<RenderTarget>		create_render_target	(const RenderTargetParameters& parameters) = 0;
		virtual UPtr<ConstantBuffer>	create_constant_buffer	(const ConstantBufferParameters& parameters) = 0;
		virtual UPtr<BlendState>		create_blend_state		(const BlendStateParameters& parameters) = 0;
		virtual UPtr<SamplerState>		create_sampler_state	(const SamplerStateParameters& parameters) = 0;
		virtual UPtr<Texture>			create_texture			(const TextureParameters& parameters) = 0;

		// Accessors
		virtual Device&					device					() = 0;
		virtual const Device&			device					() const = 0;
		virtual DeviceContext&			device_context			() = 0;
		virtual const DeviceContext&	device_context			() const = 0;
	};
}