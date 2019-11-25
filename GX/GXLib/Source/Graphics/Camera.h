#pragma once

// Project Includes
#include <GXLib/Math/Vector.h>
#include <GXLib/API.h>

namespace GX
{
	class GXLIBS_API Camera
	{
	public:
		// Construction & Destruction
						Camera					();
						Camera					(const Vector3F& position, const Vector3F& target, const Vector3F& up);

		// Properties
		void			set_position			(const Vector3F& position);
		const Vector3F& position				() const;

		void			set_target				(const Vector3F& target);
		const Vector3F& target					() const;

		void			set_up					(const Vector3F& up);
		const Vector3F& up						() const;

		const Vector3F& forward					() const;
		const Vector3F& right					() const;
		const Vector3F& up_real					() const;

		// Utility
		void			move_left				(float distance);
		void			move_up					(float distance);
		void			move_forward			(float distance);
		void			rotate_cw_around_target	(float angle);
		void			rotate_up_around_target	(float angle);

	private:
		// Helpers
		void			calculate_directions	();

		Vector3F m_position;
		Vector3F m_target;
		Vector3F m_up;
		Vector3F m_forward;
		Vector3F m_right;
		Vector3F m_up_real;
	};
}