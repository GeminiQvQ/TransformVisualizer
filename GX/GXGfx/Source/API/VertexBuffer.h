#pragma once

// Stdlib Includes
#include <string>
#include <vector>

// Forward Declarations
namespace GX::Gfx
{
	class DeviceContext;
	class VertexShader;
}

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	enum class VertexBufferFormat
	{
		R32G32Float,
		R32G32B32Float,
		R32G32B32A32Float,
		R32SInt
	};

	struct VertexBufferLayoutEntry
	{
		std::string			semantic;
		VertexBufferFormat	format;
		int					size;
	};
	
	struct VertexBufferParameters
	{
		std::vector<VertexBufferLayoutEntry> layout;
		const void*							 data;
		int									 size;
		int									 stride;
		VertexShader*						 shader;
	};

	//-----------------------------------------------------------------------------------------------------
	// Vertex Buffer
	//-----------------------------------------------------------------------------------------------------

	class VertexBuffer
	{
	public:
		// Construction & Destruction
		virtual			~VertexBuffer	() = default;

		// Vertex Buffer Interface
		virtual void	draw			(DeviceContext& device_context) = 0;
	};
}