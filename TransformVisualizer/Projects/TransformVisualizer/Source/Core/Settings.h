#pragma once

// Project Includes
#include <GXLib/Math/Vector.h>

// Stdlib Includes
#include <array>

// Forward Declarations
class QJsonObject;

namespace TransformVisualizer
{
	struct Settings
	{
		GX::Vector4F				background_color;
		std::array<GX::Vector4F, 6> cube_color;

		float						projection_fov;
		float						projection_near_plane;
		float						projection_far_plane;

		// Construction & Destruction
				Settings	();

		// Save & Load
		void	save		(QJsonObject& json);
		void	load		(const QJsonObject& json);
	};
}