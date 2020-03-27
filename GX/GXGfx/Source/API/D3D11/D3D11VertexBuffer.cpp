// Local Includes
#include <GXGfx/API/D3D11/D3D11VertexBuffer.h>

// Project Includes
#include <GXGfx/API/D3D11/D3D11Device.h>
#include <GXGfx/API/D3D11/D3D11DeviceContext.h>
#include <GXGfx/API/D3D11/D3D11Shader.h>
#include <GXLib/Graphics/API/Windows/WindowsPixelFormat.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct D3D11VertexBuffer::Internal
	{
		ID3D11Buffer*		buffer;
		ID3D11InputLayout*	input_layout;
		size_t				vertex_size;
		size_t				vertex_count;

		Internal(ID3D11Buffer* buffer, ID3D11InputLayout* input_layout, size_t vertex_size, size_t vertex_count)
			: buffer {buffer}
			, input_layout {input_layout}
			, vertex_size {vertex_size}
			, vertex_count {vertex_count}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Buffer* buffer, ID3D11InputLayout* input_layout, size_t vertex_size, size_t vertex_count)
		: m {std::make_unique<Internal>(buffer, input_layout, vertex_size, vertex_count)}
	{
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		m->buffer->Release();
		m->input_layout->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Vertex Buffer Interface
	//-----------------------------------------------------------------------------------------------------

	void D3D11VertexBuffer::update(DeviceContext& device_context, const void* data, size_t size)
	{
		auto& d3d11_device_context = static_cast<D3D11DeviceContext&>(device_context).d3d11_device_context();

		D3D11_MAPPED_SUBRESOURCE resource {};
		d3d11_device_context.Map(m->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, data, size);
		d3d11_device_context.Unmap(m->buffer, 0);
	}

	void D3D11VertexBuffer::draw(DeviceContext& device_context)
	{
		UINT stride = (UINT)m->vertex_size;
		UINT offset = 0;

		auto& d3d11_device_context = static_cast<D3D11DeviceContext&>(device_context).d3d11_device_context();
		d3d11_device_context.IASetInputLayout(m->input_layout);
		d3d11_device_context.IASetVertexBuffers(0, 1, &m->buffer, &stride, &offset);
		d3d11_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TODO
		d3d11_device_context.Draw((UINT)m->vertex_count, 0);
	}

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11VertexBuffer> create_vertex_buffer(const VertexBufferParameters& parameters, D3D11Device& device, D3D11DeviceContext& device_context)
	{
		ID3D11Buffer* buffer = nullptr;
		{
			D3D11_BUFFER_DESC desc {};
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = (UINT)parameters.size;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			
			const auto result = device.d3d11_device().CreateBuffer(&desc, nullptr, &buffer);
			if (FAILED(result))
			{
				return nullptr;
			}
		}

		ID3D11InputLayout* input_layout = nullptr;
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> input_descs;
			int input_offset = 0;

			for (const auto& layout : parameters.layout)
			{
				D3D11_INPUT_ELEMENT_DESC input_desc {};
				input_desc.SemanticName = layout.semantic.c_str();
				input_desc.SemanticIndex = 0;
				input_desc.Format = to_dxgi_format(layout.format);
				input_desc.InputSlot = 0;
				input_desc.AlignedByteOffset = input_offset;
				input_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				input_desc.InstanceDataStepRate = 0;
				input_descs.push_back(input_desc);
				input_offset += layout.size;
			}

			auto shader = static_cast<D3D11VertexShader*>(parameters.shader);

			const auto result = device.d3d11_device().CreateInputLayout(&input_descs[0], (UINT)input_descs.size(), shader->d3d11_blob().GetBufferPointer(), shader->d3d11_blob().GetBufferSize(), &input_layout);
			if (FAILED(result))
			{
				buffer->Release();
				return nullptr;
			}
		}

		auto vertex_buffer = std::make_unique<D3D11VertexBuffer>(buffer, input_layout, parameters.stride, parameters.size / parameters.stride);

		if (parameters.data != nullptr)
		{
			vertex_buffer->update(device_context, parameters.data, parameters.size);
		}

		return vertex_buffer;
	}
}