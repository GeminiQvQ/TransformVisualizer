// Local Includes
#include <GXQt/Utility/WidgetState/WidgetState.h>

// Project Includes
#include <GXQt/Utility/JSON.h>

// Third-Party Includes
#include <QHeaderView>
#include <QJsonObject>
#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// State Functions
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	void save(const QWidget& widget, QJsonObject& json)
	{
		static_assert(false, "Unimplemented");
	}

	template <class T>
	void load(QWidget& widget, const QJsonObject& json)
	{
		static_assert(false, "Unimplemented");
	}

	//-----------------------------------------------------------------------------------------------------

	template <>
	void save<QHeaderView>(const QWidget& widget, QJsonObject& json)
	{
		auto&& header_view = static_cast<const QHeaderView&>(widget);

		QJsonArray json_section_sizes;
		for (int i = 0; i < header_view.count() - 1; ++i)
		{
			json_section_sizes.push_back(header_view.sectionSize(i));
		}
		json["SectionSizes"] = json_section_sizes;
	}

	template <>
	void load<QHeaderView>(QWidget& widget, const QJsonObject& json)
	{
		auto&& header_view = static_cast<QHeaderView&>(widget);

		const auto json_section_sizes = json["SectionSizes"].toArray();
		for (int i = 0; i < json_section_sizes.size() && i < header_view.count(); ++i)
		{
			header_view.resizeSection(i, json_section_sizes[i].toInt());
		}
	}

	//-----------------------------------------------------------------------------------------------------

	template <>
	void save<QMainWindow>(const QWidget& widget, QJsonObject& json)
	{
		auto&& main_window = static_cast<const QMainWindow&>(widget);

		json["Geometry"] = QString{main_window.saveGeometry().toBase64()};
		json["State"] = QString{main_window.saveState().toBase64()};
	}

	template <>
	void load<QMainWindow>(QWidget& widget, const QJsonObject& json)
	{
		auto&& main_window = static_cast<QMainWindow&>(widget);
		
		main_window.restoreGeometry(QByteArray::fromBase64(json["Geometry"].toString().toLatin1()));
		main_window.restoreState(QByteArray::fromBase64(json["State"].toString().toLatin1()));
	}

	//-----------------------------------------------------------------------------------------------------

	template <>
	void save<QSplitter>(const QWidget& widget, QJsonObject& json)
	{
		auto&& splitter = static_cast<const QSplitter&>(widget);

		const auto sizes = splitter.sizes();
		QJsonArray json_sizes;
		to_json_array(json_sizes, sizes.begin(), sizes.end());
		json["Sizes"] = json_sizes;
	}

	template <>
	void load<QSplitter>(QWidget& widget, const QJsonObject& json)
	{
		auto&& splitter = static_cast<QSplitter&>(widget);

		QList<int> sizes;
		from_json_array<int>(json["Sizes"].toArray(), std::back_inserter(sizes));
		splitter.setSizes(sizes);
	}

	//-----------------------------------------------------------------------------------------------------

	template <>
	void save<QTreeView>(const QWidget& widget, QJsonObject& json)
	{
		auto&& tree_view = static_cast<const QTreeView&>(widget);

		QJsonObject json_header;
		save<QHeaderView>(*tree_view.header(), json_header);
		json["Header"] = json_header;
	}

	template <>
	void load<QTreeView>(QWidget& widget, const QJsonObject& json)
	{
		auto&& tree_view = static_cast<QTreeView&>(widget);

		auto json_header = json["Header"].toObject();
		load<QHeaderView>(*tree_view.header(), json_header);
	}

	//-----------------------------------------------------------------------------------------------------

	template <>
	void save<QWidget>(const QWidget& widget, QJsonObject& json)
	{
		json["PosX"] = widget.pos().x();
		json["PosY"] = widget.pos().y();
		json["SizeX"] = widget.width();
		json["SizeY"] = widget.height();
	}

	template <>
	void load<QWidget>(QWidget& widget, const QJsonObject& json)
	{
		widget.move(json["PosX"].toInt(), json["PosY"].toInt());
		widget.resize(json["SizeX"].toInt(), json["SizeY"].toInt());
	}

	//-----------------------------------------------------------------------------------------------------
	// State Registry
	//-----------------------------------------------------------------------------------------------------

	using SaveFunction = void(*)(const QWidget&, QJsonObject&);
	using LoadFunction = void(*)(QWidget&, const QJsonObject&);
	using StateFunctions = std::pair<SaveFunction, LoadFunction>;
	using StateFunctionRegistry = std::unordered_map<const QMetaObject*, StateFunctions>;
	
	template <class T>
	constexpr StateFunctionRegistry::value_type make_state_entry()
	{
		return std::make_pair(&T::staticMetaObject, std::make_pair(save<T>, load<T>));
	}

	//-----------------------------------------------------------------------------------------------------

	const StateFunctionRegistry& state_function_registry()
	{
		static const StateFunctionRegistry s_registry
		{
			make_state_entry<QHeaderView>(),
			make_state_entry<QMainWindow>(),
			make_state_entry<QSplitter>(),
			make_state_entry<QTreeView>(),
			make_state_entry<QWidget>()
		};
		return s_registry;
	}

	//-----------------------------------------------------------------------------------------------------

	template <size_t I, class... Args>
	void process_state_function(const QMetaObject* meta_object, Args&&... args)
	{
		if (meta_object != nullptr)
		{
			const auto it = state_function_registry().find(meta_object);
			if (it != state_function_registry().end())
			{
				std::get<I>(it->second)(std::forward<Args>(args)...);
			}

			process_state_function<I>(meta_object->superClass(), std::forward<Args>(args)...);
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Save & Load State
	//-----------------------------------------------------------------------------------------------------

	void save_widget_state(const QWidget& widget, QJsonObject& json)
	{
		process_state_function<0>(widget.metaObject(), widget, json);
	}

	void load_widget_state(QWidget& widget, const QJsonObject& json)
	{
		process_state_function<1>(widget.metaObject(), widget, json);
	}

	void register_widget_state(QWidget& widget, const QString& name)
	{
		widget.setProperty("WidgetStateID", name);
	}
}