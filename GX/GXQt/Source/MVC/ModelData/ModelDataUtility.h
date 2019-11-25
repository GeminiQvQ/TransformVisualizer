#pragma once

// Project Includes
#include <GXLib/Math/Vector4F.h>
#include <GXQt/MVC/ModelData/ModelData.h>
#include <GXQt/API.h>

// Forward Declarations
class QTreeView;

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Model Data Creators
	//-----------------------------------------------------------------------------------------------------

	GXQT_API ModelDataEntry			create_model_data_checkbox			(bool value);
	GXQT_API ModelDataEntry			create_model_data_spinbox			(int value, int value_min = -99999, int value_max = 99999);
	GXQT_API ModelDataEntry			create_model_data_spinbox			(float value, float value_min = -99999.0f, float value_max = 99999.0f);
	GXQT_API ModelDataEntry			create_model_data_lineedit			(const QString& value);
	GXQT_API ModelDataEntry			create_model_data_color_button		(const Vector4F& color);

	//-----------------------------------------------------------------------------------------------------
	// Model Data Editor Creator
	//-----------------------------------------------------------------------------------------------------

	GXQT_API MDViewDelegateEditor*	create_model_data_editor			(const ModelDataEntry& entry);

	//-----------------------------------------------------------------------------------------------------
	// View Utility
	//-----------------------------------------------------------------------------------------------------

	GXQT_API void					install_model_data_view_delegate	(QTreeView& view, bool persistent);
}