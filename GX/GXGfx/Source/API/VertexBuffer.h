#pragma once

// Project Includes
#include <GXLib/Graphics/PixelFormat.h>

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

	struct VertexBufferLayoutEntry
	{
		std::string	semantic;
		PixelFormat	format;
		int			size;
	};
	
	struct VertexBufferParameters
	{
		std::vector<VertexBufferLayoutEntry> layout;
		const void*							 data;
		size_t								 size;
		size_t								 stride;
		VertexShader*						 shader;
	};

	//-----------------------------------------------------------------------------------------------------
	// VertexBuffer
	//-----------------------------------------------------------------------------------------------------

	class VertexBuffer
	{
	public:
		// Construction & Destruction
		virtual			~VertexBuffer	() = default;

		// Vertex Buffer Interface
		virtual void	update			(DeviceContext& device_context, const void* data, size_t size) = 0;
		virtual void	draw			(DeviceContext& device_context) = 0;
	};
}