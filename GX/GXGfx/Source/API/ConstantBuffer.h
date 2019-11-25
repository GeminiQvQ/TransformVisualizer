#pragma once

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	struct ConstantBufferParameters
	{
		int size;
	};

	//-----------------------------------------------------------------------------------------------------
	// Constant Buffer
	//-----------------------------------------------------------------------------------------------------

	class ConstantBuffer
	{
	public:
		// Construction & Destruction
		virtual			~ConstantBuffer	() = default;

		// Constant Buffer Interface
		virtual void	set				(const void* data) = 0;
	};
}