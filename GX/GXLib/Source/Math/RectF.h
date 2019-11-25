#pragma once

// Project Includes
#include <GXLib/Math/Vector2F.h>
#include <GXLib/Math/Matrix.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// RectF
	//-----------------------------------------------------------------------------------------------------

	class RectF
	{
	public:
		// Construction & Destruction
					RectF		();
					RectF		(float x, float y, float w, float h);
					RectF		(const RectF& rhs);

		// Operations
		Vector2F	center		() const;
		Vector2F	tl			() const;
		Vector2F	tr			() const;
		Vector2F	bl			() const;
		Vector2F	br			() const;

		bool		is_valid	() const;

		float x, y, w, h;
	};

	//-----------------------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------------------

	RectF	operator *	(const RectF& lhs, const Matrix& rhs);
	RectF&	operator *= (RectF& lhs, const Matrix& rhs);
}

// Inline Includes
#include <GXLib/Math/RectF.inl>
