#pragma once

#include <variant>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	struct RenderTargetParametersBackbuffer
	{
	};

	using RenderTargetParameters = std::variant
	<
		RenderTargetParametersBackbuffer
	>;

	//-----------------------------------------------------------------------------------------------------
	// Render Target
	//-----------------------------------------------------------------------------------------------------

	class RenderTarget
	{
	public:
		// Construction & Destruction
		virtual ~RenderTarget() = default;
	};
}