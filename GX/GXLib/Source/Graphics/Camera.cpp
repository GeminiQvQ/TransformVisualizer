// Local Includes
#include <GXLib/Graphics/Camera.h>

// Project Includes
#include <GXLib/Math/Math.h>
#include <GXLib/Math/Matrix.h>

namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Camera::Camera()
		: Camera {Vector3F{}, Vector3F{}, Vector3F{0.0f, 1.0f, 0.0f}}
	{
	}

	Camera::Camera(const Vector3F& position, const Vector3F& target, const Vector3F& up)
		: m_position {position}
		, m_target {target}
		, m_up {up}
		, m_forward {}
		, m_right {}
		, m_up_real {}
	{
		calculate_directions();
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void Camera::set_position(const Vector3F& position)
	{
		m_position = position;
		calculate_directions();
	}

	const Vector3F& Camera::position() const
	{
		return m_position;
	}

	//-----------------------------------------------------------------------------------------------------

	void Camera::set_target(const Vector3F& target)
	{
		m_target = target;
		calculate_directions();
	}

	const Vector3F& Camera::target() const
	{
		return m_target;
	}

	//-----------------------------------------------------------------------------------------------------

	void Camera::set_up(const Vector3F& up)
	{
		m_up = up;
		calculate_directions();
	}

	const Vector3F& Camera::up() const
	{
		return m_up;
	}

	//-----------------------------------------------------------------------------------------------------

	const Vector3F& Camera::forward() const
	{
		return m_forward;
	}

	const Vector3F& Camera::right() const
	{
		return m_right;
	}

	const Vector3F& Camera::up_real() const
	{
		return m_up_real;
	}

	//-----------------------------------------------------------------------------------------------------
	// Utility
	//-----------------------------------------------------------------------------------------------------

	void Camera::move_left(float distance)
	{
		m_position -= m_right * distance;
		m_target -= m_right * distance;
		calculate_directions();
	}

	void Camera::move_up(float distance)
	{
		m_position += m_up_real * distance;
		m_target += m_up_real * distance;
		calculate_directions();
	}

	void Camera::move_forward(float distance)
	{
		m_position -= m_forward * distance;
		calculate_directions();
	}

	void Camera::rotate_cw_around_target(float angle)
	{
		const auto forward = (m_target - m_position);
		const auto transform = GX::Matrix::create_rotation(m_up_real * -angle);
		m_position = m_target - forward * transform;
		calculate_directions();
	}

	void Camera::rotate_up_around_target(float angle)
	{
		const auto forward = (m_target - m_position);
		const auto transform = GX::Matrix::create_rotation(m_right * angle);
		m_position = m_target - forward * transform;
		calculate_directions();
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void Camera::calculate_directions()
	{
		m_forward = (m_target - m_position).normalized();
		m_right = cross(m_up, m_forward).normalized();
		m_up_real = cross(m_forward, m_right).normalized();
	}
}