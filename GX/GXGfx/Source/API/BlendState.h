#pragma once

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	enum class BlendType
	{
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha
	};

	enum class BlendOp
	{
		Add
	};

	struct BlendStateParameters
	{
		bool	  enabled;
		BlendType blend_src;
		BlendType blend_dst;
		BlendOp   blend_op;
		BlendType blend_alpha_src;
		BlendType blend_alpha_dst;
		BlendOp   blend_alpha_op;
	};

	//-----------------------------------------------------------------------------------------------------
	// Blend State
	//-----------------------------------------------------------------------------------------------------

	class BlendState
	{
	public:
		// Construction & Destruction
		virtual ~BlendState() = default;
	};
}