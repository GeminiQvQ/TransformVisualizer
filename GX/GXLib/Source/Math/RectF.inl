namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline RectF::RectF()
		: RectF {0.0f, 0.0f, 0.0f, 0.0f}
	{
	}

	inline RectF::RectF(float x, float y, float w, float h)
		: x {x}
		, y {y}
		, w {w}
		, h {h}
	{
	}

	inline RectF::RectF(const RectF& rhs)
		: RectF {rhs.x, rhs.y, rhs.w, rhs.h}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	inline Vector2F RectF::center() const
	{
		return Vector2F{x + w * 0.5f, y + h * 0.5f};
	}

	inline Vector2F RectF::tl() const
	{
		return Vector2F{x, y};
	}

	inline Vector2F RectF::tr() const
	{
		return Vector2F{x + w, y};
	}

	inline Vector2F RectF::bl() const
	{
		return Vector2F{x, y + h};
	}

	inline Vector2F RectF::br() const
	{
		return Vector2F{x + w, y + h};
	}

	inline bool RectF::is_valid() const
	{
		return (w > 0.0f && h > 0.0f);
	}

	//-----------------------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------------------

	inline RectF operator * (const RectF& lhs, const Matrix& rhs)
	{
		auto result = lhs;
		result *= rhs;
		return result;
	}

	inline RectF& operator *= (RectF& lhs, const Matrix& rhs)
	{
		auto tl = lhs.tl() * rhs;
		lhs.x = tl.x;
		lhs.y = tl.y;
		return lhs;
	}
}