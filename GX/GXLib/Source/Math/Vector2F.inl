// Stdlib Includes
#include <cmath>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline Vector2F::Vector2F()
		: Vector2F {0.0f, 0.0f}
	{
	}

	inline Vector2F::Vector2F(float x, float y)
		: x {x}
		, y {y}
	{
	}

	inline Vector2F::Vector2F(const Vector2F& rhs)
		: Vector2F {rhs.x, rhs.y}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Operations
	//-----------------------------------------------------------------------------------------------------

	inline Vector2F Vector2F::normalized() const
	{
		float len = length();
		return Vector2F{x / len, y / len};
	}

	inline float Vector2F::length() const
	{
		return std::sqrt(length_sq());
	}

	inline float Vector2F::length_sq() const
	{
		return (x * x + y * y);
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Functions
	//-----------------------------------------------------------------------------------------------------

	inline float dot(const Vector2F& lhs, const Vector2F& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y);
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector2F Operators
	//-----------------------------------------------------------------------------------------------------

	inline Vector2F operator + (const Vector2F& lhs, const Vector2F& rhs)
	{
		Vector2F result = lhs;
		result += rhs;
		return result;
	}

	inline Vector2F& operator += (Vector2F& lhs, const Vector2F& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	inline Vector2F operator - (const Vector2F& lhs, const Vector2F& rhs)
	{
		Vector2F result = lhs;
		result -= rhs;
		return result;
	}

	inline Vector2F& operator -= (Vector2F& lhs, const Vector2F& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	inline Vector2F operator * (const Vector2F& lhs, float rhs)
	{
		Vector2F result = lhs;
		result *= rhs;
		return result;
	}

	inline Vector2F& operator *= (Vector2F& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}
}