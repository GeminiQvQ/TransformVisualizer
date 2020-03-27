// Local Includes
#include <GXGfx/API/D3D11/D3D11Context.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11BlendState.h>
#include <GXGfx/API/D3D11/D3D11ConstantBuffer.h>
#include <GXGfx/API/D3D11/D3D11Device.h>
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>
#include <GXGfx/API/D3D11/D3D11RenderTargetBackBuffer.h>
#include <GXGfx/API/D3D11/D3D11SamplerState.h>
#include <GXGfx/API/D3D11/D3D11Shader.h>
#include <GXGfx/API/D3D11/D3D11Texture.h>
#include <GXGfx/API/D3D11/D3D11VertexBuffer.h>
#include <GXGfx/API/Viewport.h>
#include <GXLib/Application/Window.h>
#include <GXLib/StdExt/VariantIndex.h>

// Third-Party Includes
#include <d3d11.h>
#include <d3dcompiler.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11Context::Internal
	{
		D3D11Device			device;
		D3D11DeviceContext	device_context;

		Internal(HWND hwnd)
			: device {hwnd}
			, device_context {device.d3d11_device_context()}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11Context::D3D11Context(HWND hwnd)
		: m {std::make_unique<Internal>(hwnd)}
	{
	}

	D3D11Context::~D3D11Context()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Resources
	//-----------------------------------------------------------------------------------------------------

	template <typename ShaderType, typename D3D11ShaderType, typename CreateFunction>
	UPtr<ShaderType> create_shader(ID3D11Device& d3d11_device, CreateFunction function, std::wstring_view filepath, std::string_view entry_point, std::string_view target)
	{
		ID3D10Blob* shader_blob {};
		ID3D10Blob* error_blob {};

		auto compile_result = D3DCompileFromFile(
			filepath.data(),
			nullptr,
			nullptr,
			entry_point.data(),
			target.data(),
			0,
			0,
			&shader_blob,
			&error_blob
		);

		if (FAILED(compile_result))
		{
			if (error_blob != nullptr)
			{
				auto error = (const char*)error_blob->GetBufferPointer();
				error;

				int a = 0;
				++a;
				// TODO: Use error.
			}

			return nullptr;
		}

		D3D11ShaderType* d3d11_shader {};
		auto create_result = (d3d11_device.*function)(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, &d3d11_shader);

		if (FAILED(create_result))
		{
			shader_blob->Release();
			return nullptr;
		}

		return std::make_unique<ShaderType>(d3d11_shader, shader_blob);
	}

	UPtr<VertexShader> D3D11Context::create_vertex_shader(std::wstring_view filepath)
	{
		return create_shader<D3D11VertexShader, ID3D11VertexShader>(m->device.d3d11_device(), &ID3D11Device::CreateVertexShader, filepath, "vs_main", "vs_4_0");
	}

	UPtr<PixelShader> D3D11Context::create_pixel_shader(std::wstring_view filepath)
	{
		return create_shader<D3D11PixelShader, ID3D11PixelShader>(m->device.d3d11_device(), &ID3D11Device::CreatePixelShader, filepath, "ps_main", "ps_4_0");
	}

	UPtr<VertexBuffer> D3D11Context::create_vertex_buffer(const VertexBufferParameters& parameters)
	{
		return GX::Gfx::create_vertex_buffer(parameters, m->device, m->device_context);
	}

	UPtr<RenderTarget> D3D11Context::create_render_target(const RenderTargetParameters& parameters)
	{
		switch (parameters.index())
		{
		case VariantIndex<RenderTargetParametersBackbuffer, RenderTargetParameters>():
			return std::make_unique<D3D11RenderTargetBackBuffer>(m->device);
		default:
			return nullptr;
		}
	}

	UPtr<ConstantBuffer> D3D11Context::create_constant_buffer(const ConstantBufferParameters& parameters)
	{
		return std::make_unique<D3D11ConstantBuffer>(parameters, m->device, m->device_context);
	}

	UPtr<BlendState> D3D11Context::create_blend_state(const BlendStateParameters& parameters)
	{
		return std::make_unique<D3D11BlendState>(parameters, m->device);
	}

	UPtr<SamplerState> D3D11Context::create_sampler_state(const SamplerStateParameters& parameters)
	{
		return GX::Gfx::create_sampler_state(parameters, m->device);
	}

	UPtr<Texture> D3D11Context::create_texture(const TextureParameters& parameters)
	{
		return GX::Gfx::create_texture(parameters, m->device);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	Device& D3D11Context::device()
	{
		return m->device;
	}

	const Device& D3D11Context::device() const
	{
		return m->device;
	}

	DeviceContext& D3D11Context::device_context()
	{
		return m->device_context;
	}

	const DeviceContext& D3D11Context::device_context() const
	{
		return m->device_context;
	}
}