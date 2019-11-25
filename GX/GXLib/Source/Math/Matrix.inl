// Project Includes
#include <GXLib/Math/Math.h>

// Stdlib Includes
#include <cmath>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline Matrix::Matrix()
		: Matrix({0.0f})
	{
	}

	inline Matrix::Matrix(const Data& data)
		: m_data {data}
	{
	}

	inline Matrix::Matrix(const Matrix& rhs)
		: Matrix {rhs.m_data}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	inline Matrix::Data& Matrix::data()
	{
		return m_data;
	}

	inline const Matrix::Data& Matrix::data() const
	{
		return m_data;
	}

	//-----------------------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------------------

	inline float& Matrix::operator [] (int index)
	{
		return m_data[index];
	}

	inline float Matrix::operator [] (int index) const
	{
		return m_data[index];
	}

	inline float Matrix::operator () (int index) const
	{
		return m_data[index];
	}

	inline float Matrix::operator () (int row, int column) const
	{
		return m_data[row * 4 + column];
	}

	//-----------------------------------------------------------------------------------------------------
	// Creators
	//-----------------------------------------------------------------------------------------------------

	inline Matrix Matrix::create_identity()
	{
		return Matrix(
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		});
	}

	inline Matrix Matrix::create_translation(const Vector3F& translation)
	{
		return Matrix(
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			translation.x, translation.y, translation.z, 1.0f
		});
	}

	inline Matrix Matrix::create_rotation(const Vector3F& angles)
	{
		const float ax = deg2rad(angles.x);
		const float ay = deg2rad(angles.y);
		const float az = deg2rad(angles.z);

		const float sx = std::sin(ax);
		const float cx = std::cos(ax);
		const float sy = std::sin(ay);
		const float cy = std::cos(ay);
		const float sz = std::sin(az);
		const float cz = std::cos(az);

		const float v00 = cy * cz;
		const float v01 = -cy * sz * cx + sy * sx;
		const float v02 = cy * sz * sx + sy * cx;
		const float v10 = sz;
		const float v11 = cz * cx;
		const float v12 = -cz * sx;
		const float v20 = -sy * cz;
		const float v21 = sy * sz * cx + cy * sx;
		const float v22 = -sy * sz * sx + cy * cx;

		return Matrix(
		{
			v00,  v01,  v02,  0.0f,
			v10,  v11,  v12,  0.0f,
			v20,  v21,  v22,  0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		});
	}

	inline Matrix Matrix::create_rotation_x(float angle)
	{
		float rada = deg2rad(angle);
		float sina = std::sin(rada);
		float cosa = std::cos(rada);

		return Matrix(
		{
			1.0f, 0.0f,  0.0f, 0.0f,
			0.0f, cosa, -sina, 0.0f,
			0.0f, sina,  cosa, 0.0f,
			0.0f, 0.0f,  0.0f, 1.0f,
		});
	}

	inline Matrix Matrix::create_rotation_y(float angle)
	{
		float rada = deg2rad(angle);
		float sina = std::sin(rada);
		float cosa = std::cos(rada);

		return Matrix(
		{
			cosa, 0.0f, -sina, 0.0f,
			0.0f, 1.0f,  0.0f, 0.0f,
			sina, 0.0f,  cosa, 0.0f,
			0.0f, 0.0f,  0.0f, 1.0f,
		});
	}

	inline Matrix Matrix::create_rotation_z(float angle)
	{
		float rada = deg2rad(angle);
		float sina = std::sin(rada);
		float cosa = std::cos(rada);

		return Matrix(
		{
			cosa, -sina, 0.0f, 0.0f,
			sina,  cosa, 0.0f, 0.0f,
			0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.0f, 0.0f, 1.0f
		});
	}

	inline Matrix Matrix::create_scale(const Vector3F& scale)
	{
		return Matrix(
		{
			scale.x, 0.0f,	  0.0f,    0.0f,
			0.0f,	 scale.y, 0.0f,    0.0f,
			0.0f,	 0.0f,	  scale.z, 0.0f,
			0.0f,	 0.0f,	  0.0f,    1.0f
		});
	}

	inline Matrix Matrix::create_perspective(float fov, float aspect, float z_near, float z_far)
	{
		float scale_y = 1.0f / std::tanf(fov * 0.5f);
		float scale_x = scale_y / aspect;

		float v00 = scale_x;
		float v11 = scale_y;
		float v22 = z_far / (z_far - z_near);
		float v32 = -z_near * z_far / (z_far - z_near);

		return Matrix(
		{
			v00,  0.0f, 0.0f, 0.0f,
			0.0f, v11,  0.0f, 0.0f,
			0.0f, 0.0f, v22,  1.0f,
			0.0f, 0.0f, v32,  0.0f
		});
	}

	inline Matrix Matrix::create_orthographic(const Vector2F& size, float z_near, float z_far)
	{
		float v00 = 2.0f / size.x;
		float v11 = 2.0f / size.y;
		float v22 = 1.0f / (z_far - z_near);
		float v32 = -z_near / (z_far - z_near);

		return Matrix(
		{
			v00,  0.0f, 0.0f, 0.0f,
			0.0f, v11,  0.0f, 0.0f,
			0.0f, 0.0f, v22,  0.0f,
			0.0f, 0.0f, v32,  1.0f
		});
	}

	inline Matrix Matrix::create_view(const Vector3F& eye, const Vector3F& at, const Vector3F& up)
	{
		auto z = (at - eye).normalized();
		auto x = cross(up, z).normalized();
		auto y = cross(z, x);

		float v30 = -dot(x, eye);
		float v31 = -dot(y, eye);
		float v32 = -dot(z, eye);

		return Matrix(
		{
			x.x, y.x, z.x, 0.0f,
			x.y, y.y, z.y, 0.0f,
			x.z, y.z, z.z, 0.0f,
			v30, v31, v32, 1.0f
		});
	}

	//-----------------------------------------------------------------------------------------------------
	// Matrix Functions
	//-----------------------------------------------------------------------------------------------------

	inline Vector3F extract_translation(const Matrix& matrix)
	{
		return Vector3F{matrix[12], matrix[13], matrix[14]};
	}

	inline Vector3F extract_rotation(const Matrix& matrix)
	{
		const auto scale_inv = 1.0f / extract_scale(matrix);
		
		const auto v00 = matrix(0, 0) * scale_inv.x;
		const auto v10 = matrix(1, 0) * scale_inv.x;
		const auto v20 = matrix(2, 0) * scale_inv.x;
		const auto v21 = matrix(2, 1) * scale_inv.y;
		const auto v22 = matrix(2, 2) * scale_inv.z;
		
		const auto ax = std::atan2(v21, v22);
		const auto ay = std::atan2(-v20, std::sqrt(v21 * v21 + v22 * v22));
		const auto az = std::atan2(v10, v00);

		return Vector3F{rad2deg(ax), rad2deg(ay), rad2deg(az)};
	}

	inline Vector3F extract_scale(const Matrix& matrix)
	{
		const auto r0 = Vector3F{matrix(0, 0), matrix(0, 1), matrix(0, 2)};
		const auto r1 = Vector3F{matrix(1, 0), matrix(1, 1), matrix(1, 2)};
		const auto r2 = Vector3F{matrix(2, 0), matrix(2, 1), matrix(2, 2)};
		
		return Vector3F{r0.length(), r1.length(), r2.length()};
	}

	//-----------------------------------------------------------------------------------------------------
	// Matrix Operators
	//-----------------------------------------------------------------------------------------------------

	inline Matrix operator * (const Matrix& lhs, const Matrix& rhs)
	{
		float v00 = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3) * rhs(3, 0);
		float v01 = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3) * rhs(3, 1);
		float v02 = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3) * rhs(3, 2);
		float v03 = lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3) * rhs(3, 3);
		float v10 = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3) * rhs(3, 0);
		float v11 = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3) * rhs(3, 1);
		float v12 = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3) * rhs(3, 2);
		float v13 = lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3) * rhs(3, 3);
		float v20 = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3) * rhs(3, 0);
		float v21 = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3) * rhs(3, 1);
		float v22 = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3) * rhs(3, 2);
		float v23 = lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3) * rhs(3, 3);
		float v30 = lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3) * rhs(3, 0);
		float v31 = lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3) * rhs(3, 1);
		float v32 = lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3) * rhs(3, 2);
		float v33 = lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3) * rhs(3, 3);

		return Matrix(
		{
			v00, v01, v02, v03,
			v10, v11, v12, v13,
			v20, v21, v22, v23,
			v30, v31, v32, v33,
		});
	}

	inline Matrix& operator *= (Matrix& lhs, const Matrix& rhs)
	{
		lhs = (lhs * rhs);
		return lhs;
	}

	inline Vector2F operator * (const Vector2F& lhs, const Matrix& rhs)
	{
		auto v4 = Vector4F{lhs.x, lhs.y, 0.0f, 1.0f} * rhs;
		return Vector2F{v4.x, v4.y};
	}

	inline Vector2F& operator *= (Vector2F& lhs, const Matrix& rhs)
	{
		lhs = (lhs * rhs);
		return lhs;
	}

	inline Vector3F operator * (const Vector3F& lhs, const Matrix& rhs)
	{
		auto v4 = Vector4F{lhs.x, lhs.y, lhs.z, 1.0f} * rhs;
		return Vector3F{v4.x, v4.y, v4.z};
	}

	inline Vector3F& operator *= (Vector3F& lhs, const Matrix& rhs)
	{
		lhs = (lhs * rhs);
		return lhs;
	}

	inline Vector4F operator * (const Vector4F& lhs, const Matrix& rhs)
	{
		float x = lhs.x * rhs(0, 0) + lhs.y * rhs(1, 0) + lhs.z * rhs(2, 0) + lhs.w * rhs(3, 0);
		float y = lhs.x * rhs(0, 1) + lhs.y * rhs(1, 1) + lhs.z * rhs(2, 1) + lhs.w * rhs(3, 1);
		float z = lhs.x * rhs(0, 2) + lhs.y * rhs(1, 2) + lhs.z * rhs(2, 2) + lhs.w * rhs(3, 2);
		float w = lhs.x * rhs(0, 3) + lhs.y * rhs(1, 3) + lhs.z * rhs(2, 3) + lhs.w * rhs(3, 3);
		return Vector4F{x, y, z, w};
	}

	inline Vector4F& operator *= (Vector4F& lhs, const Matrix& rhs)
	{
		lhs = (lhs * rhs);
		return lhs;
	}
}