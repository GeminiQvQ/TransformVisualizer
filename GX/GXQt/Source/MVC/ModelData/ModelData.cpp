// Local Includes
#include <GXQt/MVC/ModelData/ModelData.h>

// Project Includes
#include <GXQt/MVC/ModelData/ModelDataUtility.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	ModelData::ModelData()
		: m_editor_creator {create_model_data_editor}
		, m_committed {false}
	{
	}

	ModelData::ModelData(std::initializer_list<ModelDataEntry> args)
		: QVector<ModelDataEntry> {args}
		, m_editor_creator {create_model_data_editor}
		, m_committed {false}
	{
	}
}