// Third-Party Includes
#include <QVariant>

namespace GX::QT
{
	template <class It>
	void to_json_array(QJsonArray& json_array, It in0, It in1)
	{
		for (auto it = in0; it != in1; ++it)
		{
			json_array.push_back(*it);
		}
	}

	template <class T, class It>
	void from_json_array(const QJsonArray& json_array, It out)
	{
		for (auto&& json_entry : json_array)
		{
			*(out++) = json_entry.toVariant().value<T>();
		}
	}
}