#pragma once

// Third-Party Includes
#include <QJsonArray>

namespace GX::QT
{
	template <class It>
	void to_json_array(QJsonArray& json_array, It in0, It in1);

	template <class T, class It>
	void from_json_array(const QJsonArray& json_array, It out);
}

// Inline Includes
#include <GXQt/Utility/JSON.inl>