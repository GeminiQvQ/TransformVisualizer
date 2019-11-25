#pragma once

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Vector3F
	//-----------------------------------------------------------------------------------------------------

	class Vector3F
	{
	public:
		// Construction & Destruction
					Vector3F	();
					Vector3F	(float x, float y, float z);
					Vector3F	(const Vector3F& rhs);

		// Operations
		Vector3F	normalized	() const;
		float		length		() const;
		float		length_sq	() const;

		float x, y, z;
	};

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Functions
	//-----------------------------------------------------------------------------------------------------

	float	 dot	(const Vector3F& lhs, const Vector3F& rhs);
	Vector3F cross	(const Vector3F& lhs, const Vector3F& rhs);

	//-----------------------------------------------------------------------------------------------------
	// Vector3F Operators
	//-----------------------------------------------------------------------------------------------------

	Vector3F	operator +	(const Vector3F& lhs, const Vector3F& rhs);
	Vector3F&	operator += (Vector3F& lhs, const Vector3F& rhs);
	Vector3F	operator -	(const Vector3F& lhs, const Vector3F& rhs);
	Vector3F&	operator -= (Vector3F& lhs, const Vector3F& rhs);
	Vector3F	operator *	(const Vector3F& lhs, float rhs);
	Vector3F	operator *	(float lhs, const Vector3F& rhs);
	Vector3F&	operator *= (Vector3F& lhs, float rhs);
	Vector3F	operator /	(const Vector3F& lhs, float rhs);
	Vector3F	operator /	(float lhs, const Vector3F& rhs);
	Vector3F&	operator /= (Vector3F& lhs, float rhs);
}

// Inline Includes
#include <GXLib/Math/Vector3F.inl>
