// Local Includes
#include <GXGfx/API/D3D11/D3D11VertexBuffer.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>
#include <GXGfx/API/D3D11/D3D11Shader.h>

// Stdlib Includes
#include <map>

// Third-Party Includes
#include <d3d11.h>

namespace
{
	//-----------------------------------------------------------------------------------------------------
	// Type Maps
	//-----------------------------------------------------------------------------------------------------

	const std::map<GX::Gfx::VertexBufferFormat, DXGI_FORMAT>& vertex_buffer_format_map()
	{
		using namespace GX::Gfx;
		static const std::map<VertexBufferFormat, DXGI_FORMAT> s_map
		{
			{ VertexBufferFormat::R32G32Float,		 DXGI_FORMAT_R32G32_FLOAT		},
			{ VertexBufferFormat::R32G32B32Float,	 DXGI_FORMAT_R32G32B32_FLOAT	},
			{ VertexBufferFormat::R32G32B32A32Float, DXGI_FORMAT_R32G32B32A32_FLOAT },
			{ VertexBufferFormat::R32SInt,			 DXGI_FORMAT_R32_SINT			}
		};
		return s_map;
	}
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11VertexBuffer::Internal
	{
		ID3D11Buffer*		buffer;
		ID3D11InputLayout*	layout;
		int					vertex_size;
		int					vertex_count;

		Internal(int vertex_size, int vertex_count)
			: buffer {}
			, layout {}
			, vertex_size {vertex_size}
			, vertex_count {vertex_count}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferParameters& parameters, D3D11Device& device)
		: m {std::make_unique<Internal>(parameters.stride, parameters.size / parameters.stride)}
	{
		D3D11_BUFFER_DESC desc {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = parameters.size;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		device.d3d11_device().CreateBuffer(&desc, nullptr, &m->buffer);

		D3D11_MAPPED_SUBRESOURCE subresource;
		device.d3d11_device_context().Map(m->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, parameters.data, parameters.size);
		device.d3d11_device_context().Unmap(m->buffer, 0);

		std::vector<D3D11_INPUT_ELEMENT_DESC> input_descs;
		int input_offset = 0;

		for (const auto& layout : parameters.layout)
		{
			D3D11_INPUT_ELEMENT_DESC input_desc {};
			input_desc.SemanticName = layout.semantic.c_str();
			input_desc.SemanticIndex = 0;
			input_desc.Format = vertex_buffer_format_map().find(layout.format)->second;
			input_desc.InputSlot = 0;
			input_desc.AlignedByteOffset = input_offset;
			input_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_desc.InstanceDataStepRate = 0;
			input_descs.push_back(input_desc);
			input_offset += layout.size;
		}

		auto shader = static_cast<D3D11VertexShader*>(parameters.shader);
		device.d3d11_device().CreateInputLayout(&input_descs[0], (UINT)input_descs.size(), shader->d3d11_blob().GetBufferPointer(), shader->d3d11_blob().GetBufferSize(), &m->layout);
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		m->buffer->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Vertex Buffer Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11VertexBuffer::draw(DeviceContext& device_context)
	{
		UINT stride = m->vertex_size;
		UINT offset = 0;

		auto& d3d11_device_context = static_cast<D3D11DeviceContext&>(device_context).d3d11_device_context();
		d3d11_device_context.IASetInputLayout(m->layout);
		d3d11_device_context.IASetVertexBuffers(0, 1, &m->buffer, &stride, &offset);
		d3d11_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TODO
		d3d11_device_context.Draw(m->vertex_count, 0);
	}
}