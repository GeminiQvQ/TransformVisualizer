#pragma once

// Project Includes
#include <GXGfx/API/VertexBuffer.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace GX::Gfx
{
	class D3D11Device;
	class D3D11DeviceContext;
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// D3D11VertexBuffer
	//-----------------------------------------------------------------------------------------------------

	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		// Construction & Destruction
						D3D11VertexBuffer	(ID3D11Buffer* buffer, ID3D11InputLayout* input_layout, size_t vertex_size, size_t vertex_count);
						~D3D11VertexBuffer	();

		// Vertex Buffer Interface
		virtual void	update				(DeviceContext& device_context, const void* data, size_t size) override;
		virtual void	draw				(DeviceContext& device_context) override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Creator
	//-----------------------------------------------------------------------------------------------------

	UPtr<D3D11VertexBuffer> create_vertex_buffer(const VertexBufferParameters& parameters, D3D11Device& device, D3D11DeviceContext& device_context);
}