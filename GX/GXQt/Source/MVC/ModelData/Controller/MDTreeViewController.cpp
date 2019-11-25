// Local Includes
#include <GXQt/MVC/ModelData/Controller/MDTreeViewController.h>

// Project Includes
#include <GXQt/MVC/ModelData/Controller/MDModelController.h>
#include <GXQt/MVC/ModelData/ModelData.h>

// Third-Party Includes
#include <QTreeView>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct MDTreeViewController::Internal
	{
		QTreeView&			view;
		MDModelController	model_controller;

		Internal(QTreeView& view)
			: view {view}
			, model_controller {*view.model()}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	MDTreeViewController::MDTreeViewController(QTreeView& view, QObject* parent)
		: QObject {parent}
		, m {std::make_unique<Internal>(view)}
	{
		QModelIndexList indices;

		for (int r = 0; r < view.model()->rowCount(); ++r)
		{
			for (int c = 0; c < view.model()->columnCount(); ++c)
			{
				indices.push_back(view.model()->index(r, c));
			}
		}

		toggle_editors(indices);

		connect(&m->model_controller, &MDModelController::signal_request_editors, this, &MDTreeViewController::slot_request_editors);
	}

	MDTreeViewController::~MDTreeViewController()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Model Controller Slots
	//-----------------------------------------------------------------------------------------------------

	void MDTreeViewController::slot_request_editors(const QModelIndexList& indices)
	{
		toggle_editors(indices);
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void MDTreeViewController::toggle_editors(const QModelIndexList& indices)
	{
		for (const auto& index : indices)
		{
			if (index.data(ModelDataRole).isValid())
			{
				m->view.closePersistentEditor(index);
				m->view.openPersistentEditor(index);
			}
		}
	}
}