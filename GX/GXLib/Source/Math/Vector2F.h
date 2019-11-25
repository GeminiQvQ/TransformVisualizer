#pragma once

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Vector2F
	//-----------------------------------------------------------------------------------------------------

	class Vector2F
	{
	public:
		// Construction & Destruction
					Vector2F	();
					Vector2F	(float x, float y);
					Vector2F	(const Vector2F& rhs);

		// Operations
		Vector2F	normalized	() const;
		float		length		() const;
		float		length_sq	() const;

		float x, y;
	};

	//-----------------------------------------------------------------------------------------------------
	// Vector2F Functions
	//-----------------------------------------------------------------------------------------------------

	float dot(const Vector2F& lhs, const Vector2F& rhs);

	//-----------------------------------------------------------------------------------------------------
	// Vector2F Operators
	//-----------------------------------------------------------------------------------------------------

	Vector2F	operator +	(const Vector2F& lhs, const Vector2F& rhs);
	Vector2F&	operator += (Vector2F& lhs, const Vector2F& rhs);
	Vector2F	operator -	(const Vector2F& lhs, const Vector2F& rhs);
	Vector2F&	operator -= (Vector2F& lhs, const Vector2F& rhs);
	Vector2F	operator *	(const Vector2F& lhs, float rhs);
	Vector2F&	operator *= (Vector2F& lhs, float rhs);
}

// Inline Includes
#include <GXLib/Math/Vector2F.inl>
