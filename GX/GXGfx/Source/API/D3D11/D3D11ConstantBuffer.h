#pragma once

// Project Includes
#include <GXGfx/API/ConstantBuffer.h>
#include <GXGfx/API.h>

// Stdlib Includes
#include <memory>

// Forward Declarations
struct ID3D11Buffer;

namespace GX::Gfx
{
	class D3D11Device;
	class D3D11DeviceContext;
}

namespace GX::Gfx
{
	class D3D11ConstantBuffer : public ConstantBuffer
	{
	public:
		// Construction & Destruction
						D3D11ConstantBuffer		(const ConstantBufferParameters& parameters, D3D11Device& device, D3D11DeviceContext& device_context);
						~D3D11ConstantBuffer	();

		// Constant Buffer Interface
		virtual void	set						(const void* data) override;

		// Accessors
		ID3D11Buffer&	d3d11_buffer			();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}