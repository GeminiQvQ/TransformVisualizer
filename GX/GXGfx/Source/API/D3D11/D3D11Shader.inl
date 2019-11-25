namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	template <typename BaseClass, typename D3D11ShaderType>
	D3D11Shader<BaseClass, D3D11ShaderType>::D3D11Shader(D3D11ShaderType* d3d11_shader, ID3D10Blob* d3d11_blob)
		: m_d3d11_shader {d3d11_shader}
		, m_d3d11_blob {d3d11_blob}
	{
	}

	template <typename BaseClass, typename D3D11ShaderType>
	D3D11Shader<BaseClass, D3D11ShaderType>::~D3D11Shader()
	{
		m_d3d11_shader->Release();
		m_d3d11_blob->Release();
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	template <typename BaseClass, typename D3D11ShaderType>
	D3D11ShaderType& D3D11Shader<BaseClass, D3D11ShaderType>::d3d11_shader()
	{
		return *m_d3d11_shader;
	}

	template <typename BaseClass, typename D3D11ShaderType>
	ID3D10Blob& D3D11Shader<BaseClass, D3D11ShaderType>::d3d11_blob()
	{
		return *m_d3d11_blob;
	}
}