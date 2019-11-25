// Local Includes
#include <GXGfx/API/D3D11/D3D11ConstantBuffer.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11ConstantBuffer::Internal
	{
		D3D11DeviceContext& device_context;
		ID3D11Buffer*		d3d11_buffer;
		int					size;

		Internal(const ConstantBufferParameters& parameters, D3D11DeviceContext& device_context)
			: device_context(device_context)
			, d3d11_buffer {}
			, size {parameters.size}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11ConstantBuffer::D3D11ConstantBuffer(const ConstantBufferParameters& parameters, D3D11Device& device, D3D11DeviceContext& device_context)
		: m {std::make_unique<Internal>(parameters, device_context)}
	{
		D3D11_BUFFER_DESC desc {};
		desc.ByteWidth = parameters.size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		device.d3d11_device().CreateBuffer(&desc, nullptr, &m->d3d11_buffer);
	}

	D3D11ConstantBuffer::~D3D11ConstantBuffer()
	{
		m->d3d11_buffer->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Constant Buffer Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11ConstantBuffer::set(const void* data)
	{
		D3D11_MAPPED_SUBRESOURCE subresource;
		m->device_context.d3d11_device_context().Map(m->d3d11_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, data, m->size);
		m->device_context.d3d11_device_context().Unmap(m->d3d11_buffer, 0);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	ID3D11Buffer& D3D11ConstantBuffer::d3d11_buffer()
	{
		return *m->d3d11_buffer;
	}
}