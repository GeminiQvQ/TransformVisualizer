#pragma once

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Vector4F
	//-----------------------------------------------------------------------------------------------------

	class Vector4F
	{
	public:
		// Construction & Destruction
					Vector4F	();
					Vector4F	(float x, float y, float z, float w);
					Vector4F	(const Vector4F& rhs);

		// Operations
		Vector4F	normalized	() const;
		float		length		() const;
		float		length_sq	() const;

		float x, y, z, w;
	};

	//-----------------------------------------------------------------------------------------------------
	// Vector4F Functions
	//-----------------------------------------------------------------------------------------------------

	float dot(const Vector4F& lhs, const Vector4F& rhs);

	//-----------------------------------------------------------------------------------------------------
	// Vector4F Operators
	//-----------------------------------------------------------------------------------------------------

	Vector4F	operator +	(const Vector4F& lhs, const Vector4F& rhs);
	Vector4F&	operator += (Vector4F& lhs, const Vector4F& rhs);
	Vector4F	operator -	(const Vector4F& lhs, const Vector4F& rhs);
	Vector4F&	operator -= (Vector4F& lhs, const Vector4F& rhs);
	Vector4F	operator *	(const Vector4F& lhs, float rhs);
	Vector4F&	operator *= (Vector4F& lhs, float rhs);

	bool		operator == (const Vector4F& lhs, const Vector4F& rhs);
	bool		operator != (const Vector4F& lhs, const Vector4F& rhs);
}

// Inline Includes
#include <GXLib/Math/Vector4F.inl>
