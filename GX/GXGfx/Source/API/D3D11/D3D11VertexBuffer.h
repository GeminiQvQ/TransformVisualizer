#pragma once

// Project Includes
#include <GXGfx/API/VertexBuffer.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
namespace GX::Gfx
{
	class D3D11Device;
}

namespace GX::Gfx
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		// Construction & Destruction
						D3D11VertexBuffer	(const VertexBufferParameters& parameters, D3D11Device& device);
						~D3D11VertexBuffer	();

		// Vertex Buffer Interface
		virtual void	draw				(DeviceContext& device_context) override;

	private:
		struct Internal;
		const std::unique_ptr<Internal> m;
	};
}