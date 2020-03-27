#pragma once

// Project Includes
#include <GXGfx/System/Sprite/SpriteTypes.h>
#include <GXGfx/API.h>
#include <GXLib/Math/Vector.h>

// Stdlib Includes
#include <cstdint>
#include <vector>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	using FontHandle = uint32_t;
	constexpr FontHandle FontHandleInvalid = FontHandle(-1);

	struct TextInstanceParameters
	{
		Vector2F				position;
		std::vector<Vector4F>	colors;
		SpriteSpace				space;
		float					wrap_size;
		int						z_order;

		GXGFX_API TextInstanceParameters();
	};
}