// Local Includes
#include <TransformVisualizer/Core/Settings.h>

// Third-Party Includes
#include <QJsonArray>
#include <QJsonObject>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Settings::Settings()
		: background_color {0.1f, 0.1f, 0.1f, 1.0f}
		, cube_color {GX::Vector4F{1.0f, 0.0f, 0.0f, 1.0f},
					  GX::Vector4F{0.0f, 1.0f, 0.0f, 1.0f}, 
					  GX::Vector4F{0.0f, 0.0f, 1.0f, 1.0f}, 
					  GX::Vector4F{1.0f, 1.0f, 0.0f, 1.0f}, 
					  GX::Vector4F{0.0f, 1.0f, 1.0f, 1.0f}, 
					  GX::Vector4F{1.0f, 1.0f, 1.0f, 1.0f}}
		, projection_fov {60.0f}
		, projection_near_plane {0.1f}
		, projection_far_plane {1000.0f}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Save & Load
	//-----------------------------------------------------------------------------------------------------

	void Settings::save(QJsonObject& json)
	{
		const auto serialize_color = [] (const GX::Vector4F& color)
		{
			QJsonArray json_color;
			json_color.push_back(color.x);
			json_color.push_back(color.y);
			json_color.push_back(color.z);
			json_color.push_back(color.w);
			return json_color;
		};

		json["BackgroundColor"] = serialize_color(background_color);
		json["CubeColor0"] = serialize_color(cube_color[0]);
		json["CubeColor1"] = serialize_color(cube_color[1]);
		json["CubeColor2"] = serialize_color(cube_color[2]);
		json["CubeColor3"] = serialize_color(cube_color[3]);
		json["CubeColor4"] = serialize_color(cube_color[4]);
		json["CubeColor5"] = serialize_color(cube_color[5]);
		json["ProjectionFoV"] = projection_fov;
		json["ProjectionPlaneNear"] = projection_near_plane;
		json["ProjectionPlaneFar"] = projection_far_plane;
	}

	void Settings::load(const QJsonObject& json)
	{
		const auto deserialize_color = [&json] (const QString& key, GX::Vector4F& color)
		{
			const auto json_color = json[key].toArray();
			if (json_color.size() == 4)
			{
				color.x = json_color[0].toDouble();
				color.y = json_color[1].toDouble();
				color.z = json_color[2].toDouble();
				color.w = json_color[3].toDouble();
			}
		};

		deserialize_color("BackgroundColor", background_color);
		deserialize_color("CubeColor0", cube_color[0]);
		deserialize_color("CubeColor1", cube_color[1]);
		deserialize_color("CubeColor2", cube_color[2]);
		deserialize_color("CubeColor3", cube_color[3]);
		deserialize_color("CubeColor4", cube_color[4]);
		deserialize_color("CubeColor5", cube_color[5]);
		projection_fov = json["ProjectionFoV"].toDouble(projection_fov);
		projection_near_plane = json["ProjectionPlaneNear"].toDouble(projection_near_plane);
		projection_far_plane = json["ProjectionPlaneFar"].toDouble(projection_far_plane);
	}
}