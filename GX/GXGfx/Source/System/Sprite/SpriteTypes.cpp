// Local Includes
#include <GXGfx/System/Sprite/SpriteTypes.h>

// Stdlib Includes
#include <vector>

namespace GX::Gfx
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	SpriteInstanceParameters::SpriteInstanceParameters()
		: position {}
		, orientation {}
		, size {}
		, anchor {SpriteAnchor::Center}
		, space {SpriteSpace::World}
		, blend_color {1.0f, 1.0f, 1.0f, 1.0f}
		, z_order {}
		, is_grayscale {false}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------------------

	Vector2F get_anchor_offset_from_center(SpriteAnchor anchor)
	{
		static const std::vector<Vector2F> s_offsets
		{
			{  0.5f, -0.5f },
			{  0.0f, -0.5f },
			{ -0.5f, -0.5f },
			{  0.5f,  0.0f },
			{  0.0f,  0.0f },
			{ -0.5f,  0.0f },
			{  0.5f,  0.5f },
			{  0.0f,  0.5f },
			{ -0.5f,  0.5f }
		};
		return s_offsets[(size_t)anchor];
	}

	Vector2F get_anchor_offset_from_topleft(SpriteAnchor anchor)
	{
		static const std::vector<Vector2F> s_offsets
		{
			{ 0.0f,  0.0f },
			{ 0.5f,  0.0f },
			{ 1.0f,  0.0f },
			{ 0.0f, -0.5f },
			{ 0.5f, -0.5f },
			{ 1.0f, -0.5f },
			{ 0.0f, -1.0f },
			{ 0.5f, -1.0f },
			{ 1.0f, -1.0f }
		};
		return s_offsets[(size_t)anchor];
	}

}