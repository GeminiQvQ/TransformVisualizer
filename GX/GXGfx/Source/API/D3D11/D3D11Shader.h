#pragma once

// Project Includes
#include <GXGfx/API/Shader.h>

// Third-Party Includes
#include <d3d11.h>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Shader (Base)
	//-----------------------------------------------------------------------------------------------------

	template <typename BaseClass, typename D3D11ShaderType>
	class D3D11Shader : public BaseClass
	{
	public:
		// Construction & Destruction
							D3D11Shader		(D3D11ShaderType* d3d11_shader, ID3D10Blob* d3d11_blob);
							~D3D11Shader	();

		// Accessors
		D3D11ShaderType&	d3d11_shader	();
		ID3D10Blob&			d3d11_blob		();

	private:
		D3D11ShaderType*	m_d3d11_shader;
		ID3D10Blob*			m_d3d11_blob;
	};

	//-----------------------------------------------------------------------------------------------------
	// Shader Types
	//-----------------------------------------------------------------------------------------------------

	using D3D11VertexShader = D3D11Shader<VertexShader, ID3D11VertexShader>;
	using D3D11PixelShader  = D3D11Shader<PixelShader, ID3D11PixelShader>;
}

// Inline Includes
#include <GXGfx/API/D3D11/D3D11Shader.inl>
