#pragma once

// Project Includes
#include <GXGfx/API.h>
#include <GXLib/Math/Vector.h>

// Stdlib Includes
#include <cstdint>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	using SpriteHandle = uint32_t;
	constexpr SpriteHandle SpriteHandleInvalid = SpriteHandle(-1);

	enum class SpriteAnchor
	{
		TopLeft,
		Top,
		TopRight,
		Left,
		Center,
		Right,
		BottomLeft,
		Bottom,
		BottomRight
	};

	enum class SpriteSpace
	{
		World,
		Screen
	};

	struct SpriteInstanceParameters
	{
		Vector2F		position;
		float			orientation;
		Vector2F		size;
		SpriteAnchor	anchor;
		SpriteSpace		space;
		Vector4F		blend_color;
		int				z_order;
		bool			is_grayscale;

		GXGFX_API SpriteInstanceParameters();
	};

	//-----------------------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------------------

	Vector2F get_anchor_offset_from_center	(SpriteAnchor anchor);
	Vector2F get_anchor_offset_from_topleft	(SpriteAnchor anchor);
}