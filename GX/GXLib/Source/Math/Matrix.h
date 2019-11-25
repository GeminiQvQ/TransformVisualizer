#pragma once

// Project Includes
#include <GXLib/Math/Vector.h>

// Stdlib Includes
#include <array>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Matrix
	//-----------------------------------------------------------------------------------------------------

	class Matrix
	{
	public:
		using Data = std::array<float, 16>;

	public:
		// Construction & Destruction
						Matrix				();
						Matrix				(const Data& data);
						Matrix				(const Matrix& rhs);

		// Accessors
		Data&			data				();
		const Data&		data				() const;

		// Operators
		float&			operator []			(int index);
		float			operator []			(int index) const;
		float			operator ()			(int index) const;
		float			operator ()			(int row, int column) const;

		// Creators
		static Matrix	create_identity		();
		static Matrix	create_translation	(const Vector3F& translation);
		static Matrix	create_rotation		(const Vector3F& angles);
		static Matrix	create_rotation_x	(float angle);
		static Matrix	create_rotation_y	(float angle);
		static Matrix	create_rotation_z	(float angle);
		static Matrix	create_scale		(const Vector3F& scale);
		static Matrix	create_perspective	(float fov, float aspect, float z_near, float z_far);
		static Matrix	create_orthographic	(const Vector2F& size, float z_near, float z_far);
		static Matrix	create_view			(const Vector3F& eye, const Vector3F& at, const Vector3F& up);

	private:
		Data m_data;
	};

	//-----------------------------------------------------------------------------------------------------
	// Matrix Functions
	//-----------------------------------------------------------------------------------------------------

	Vector3F	extract_translation	(const Matrix& matrix);
	Vector3F	extract_rotation	(const Matrix& matrix);
	Vector3F	extract_scale		(const Matrix& matrix);

	//-----------------------------------------------------------------------------------------------------
	// Matrix Operators
	//-----------------------------------------------------------------------------------------------------

	Matrix		operator *	(const Matrix& lhs, const Matrix& rhs);
	Matrix&		operator *= (Matrix& lhs, const Matrix& rhs);
	Vector2F	operator *	(const Vector2F& lhs, const Matrix& rhs);
	Vector2F&	operator *= (Vector2F& lhs, const Matrix& rhs);
	Vector3F	operator *	(const Vector3F& lhs, const Matrix& rhs);
	Vector3F&	operator *= (Vector3F& lhs, const Matrix& rhs);
	Vector4F	operator *	(const Vector4F& lhs, const Matrix& rhs);
	Vector4F&	operator *= (Vector4F& lhs, const Matrix& rhs);
}

// Inline Includes
#include <GXLib/Math/Matrix.inl>
