#pragma once

// Project Includes
#include <GXGfx/API/Context.h>
#include <GXGfx/API.h>

// Third-Party Includes
#include <windows.h>

// Forward Declarations
namespace GX
{
	class Window;
}

namespace GX::Gfx
{
	class GXGFX_API D3D11Context : public Context
	{
	public:
		// Construction & Destruction
										D3D11Context			(HWND hwnd);
										~D3D11Context			();

		// Resources
		virtual UPtr<VertexShader>		create_vertex_shader	(std::wstring_view filepath) override;
		virtual UPtr<PixelShader>		create_pixel_shader		(std::wstring_view filepath) override;
		virtual UPtr<VertexBuffer>		create_vertex_buffer	(const VertexBufferParameters& parameters) override;
		virtual UPtr<RenderTarget>		create_render_target	(const RenderTargetParameters& parameters) override;
		virtual UPtr<ConstantBuffer>	create_constant_buffer	(const ConstantBufferParameters& parameters) override;
		virtual UPtr<BlendState>		create_blend_state		(const BlendStateParameters& parameters) override;

		// Accessors
		virtual Device&					device					() override;
		virtual DeviceContext&			device_context			() override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}