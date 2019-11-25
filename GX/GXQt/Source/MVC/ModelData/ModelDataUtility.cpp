// Local Includes
#include <GXQt/MVC/ModelData/ModelDataUtility.h>

// Project Includes
#include <GXQt/MVC/ModelData/Controller/MDTreeViewController.h>
#include <GXQt/MVC/ModelData/ViewDelegate/MDViewDelegate.h>
#include <GXQt/MVC/ModelData/ViewDelegate/MDViewDelegateEditor.h>

// Third-Party Includes
#include <QTreeView>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Model Data Creators
	//-----------------------------------------------------------------------------------------------------

	ModelDataEntry create_model_data_checkbox(bool value)
	{
		return ModelDataEntry{value};
	}

	ModelDataEntry create_model_data_spinbox(int value, int value_min, int value_max)
	{
		ModelDataEntry entry{value};
		entry.set_attribute(ModelDataAttribute::ValueMin, value_min);
		entry.set_attribute(ModelDataAttribute::ValueMax, value_max);
		return entry;
	}

	ModelDataEntry create_model_data_spinbox(float value, float value_min, float value_max)
	{
		ModelDataEntry entry{value};
		entry.set_attribute(ModelDataAttribute::ValueMin, value_min);
		entry.set_attribute(ModelDataAttribute::ValueMax, value_max);
		return entry;
	}

	ModelDataEntry create_model_data_lineedit(const QString& value)
	{
		return ModelDataEntry{value};
	}

	ModelDataEntry create_model_data_color_button(const Vector4F& color)
	{
		ModelDataEntry entry{QVariant::fromValue(color)};
		entry.set_attribute(ModelDataAttribute::Color);
		return entry;
	}

	//-----------------------------------------------------------------------------------------------------
	// Model Data Editor Creator
	//-----------------------------------------------------------------------------------------------------

	MDViewDelegateEditor* create_model_data_editor(const ModelDataEntry& entry)
	{
		MDViewDelegateEditor* editor{};

		auto type = entry.value().userType();

		if (type == QMetaType::Bool)
		{
			editor = new MDEBool{};
		}

		if (type == QMetaType::Float ||
			type == QMetaType::Double)
		{
			editor = new MDEFloat{};
		}

		if (type == QMetaType::QString)
		{
			editor = new MDEString{};
		}

		if (type == qMetaTypeId<Vector4F>())
		{
			if (entry.has_attribute(ModelDataAttribute::Color))
			{
				editor = new MDEVector4FColor();
			}
		}

		editor->set_data(entry);

		return editor;
	}

	//-----------------------------------------------------------------------------------------------------
	// View Utility
	//-----------------------------------------------------------------------------------------------------

	void install_model_data_view_delegate(QTreeView& view, bool persistent)
	{
		view.setItemDelegate(new MDViewDelegate(&view));

		if (persistent)
		{
			new MDTreeViewController{view, &view};
		}
	}
}