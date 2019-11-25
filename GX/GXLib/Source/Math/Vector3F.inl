// Stdlib Includes
#include <cmath>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline Vector3F::Vector3F()
		: Vector3F(0.0f, 0.0f, 0.0f)
	{
	}

	inline Vector3F::Vector3F(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	inline Vector3F::Vector3F(const Vector3F& rhs)
		: Vector3F(rhs.x, rhs.y, rhs.z)
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Operations
	//-----------------------------------------------------------------------------------------------------

	inline Vector3F Vector3F::normalized() const
	{
		float len = length();
		return Vector3F{x / len, y / len, z / len};
	}

	inline float Vector3F::length() const
	{
		return std::sqrt(length_sq());
	}

	inline float Vector3F::length_sq() const
	{
		return (x * x + y * y + z * z);
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Functions
	//-----------------------------------------------------------------------------------------------------

	inline float dot(const Vector3F& lhs, const Vector3F& rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}

	inline Vector3F cross(const Vector3F& lhs, const Vector3F& rhs)
	{
		return Vector3F
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Operators
	//-----------------------------------------------------------------------------------------------------

	inline Vector3F operator + (const Vector3F& lhs, const Vector3F& rhs)
	{
		Vector3F result = lhs;
		result += rhs;
		return result;
	}

	inline Vector3F& operator += (Vector3F& lhs, const Vector3F& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	inline Vector3F operator - (const Vector3F& lhs, const Vector3F& rhs)
	{
		Vector3F result = lhs;
		result -= rhs;
		return result;
	}

	inline Vector3F& operator -= (Vector3F& lhs, const Vector3F& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	inline Vector3F operator * (const Vector3F& lhs, float rhs)
	{
		Vector3F result = lhs;
		result *= rhs;
		return result;
	}

	inline Vector3F operator * (float lhs, const Vector3F& rhs)
	{
		Vector3F result = rhs;
		result *= lhs;
		return result;
	}

	inline Vector3F& operator *= (Vector3F& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		return lhs;
	}

	inline Vector3F operator / (const Vector3F& lhs, float rhs)
	{
		Vector3F result = lhs;
		result /= rhs;
		return result;
	}

	inline Vector3F operator / (float lhs, const Vector3F& rhs)
	{
		Vector3F result = rhs;
		result.x = lhs / result.x;
		result.y = lhs / result.y;
		result.z = lhs / result.z;
		return result;
	}

	inline Vector3F& operator /= (Vector3F& lhs, float rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
		return lhs;
	}
}