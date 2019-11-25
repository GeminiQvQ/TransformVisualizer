// Stdlib Includes
#include <cmath>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline Vector4F::Vector4F()
		: Vector4F(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	inline Vector4F::Vector4F(float x, float y, float z, float w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{
	}

	inline Vector4F::Vector4F(const Vector4F& rhs)
		: Vector4F(rhs.x, rhs.y, rhs.z, rhs.w)
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Operations
	//-----------------------------------------------------------------------------------------------------

	inline Vector4F Vector4F::normalized() const
	{
		float len = length();
		return Vector4F{x / len, y / len, z / len, w / len};
	}

	inline float Vector4F::length() const
	{
		return std::sqrt(length_sq());
	}

	inline float Vector4F::length_sq() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Functions
	//-----------------------------------------------------------------------------------------------------

	inline float dot(const Vector4F& lhs, const Vector4F& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w);
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Operators
	//-----------------------------------------------------------------------------------------------------

	inline Vector4F operator + (const Vector4F& lhs, const Vector4F& rhs)
	{
		Vector4F result = lhs;
		result += rhs;
		return result;
	}

	inline Vector4F& operator += (Vector4F& lhs, const Vector4F& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
		return lhs;
	}

	inline Vector4F operator - (const Vector4F& lhs, const Vector4F& rhs)
	{
		Vector4F result = lhs;
		result -= rhs;
		return result;
	}

	inline Vector4F& operator -= (Vector4F& lhs, const Vector4F& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.w -= rhs.w;
		return lhs;
	}

	inline Vector4F operator * (const Vector4F& lhs, float rhs)
	{
		Vector4F result = lhs;
		result *= rhs;
		return result;
	}

	inline Vector4F& operator *= (Vector4F& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
		return lhs;
	}
}