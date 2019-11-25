#pragma once

// Project Includes
#include <GXGfx/API/RenderTarget.h>
#include <GXLib/Core/TypeAlias.h>

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
	class VertexShader;
	class VertexBuffer;
	struct BlendStateParameters;
	struct ConstantBufferParameters;
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

		// Accessors
		virtual Device&					device					() = 0;
		virtual DeviceContext&			device_context			() = 0;
	};
}