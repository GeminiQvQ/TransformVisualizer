// Local Includes
#include <GXQt/MVC/ModelData/Controller/MDModelController.h>

// Project Includes
#include <GXQt/Utility/Event.h>

// Stdlib Includes
#include <set>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct MDModelController::Internal
	{
		QAbstractItemModel&				model;
		std::set<QPersistentModelIndex> indices;
		bool							posted;

		Internal(QAbstractItemModel& model)
			: model {model}
			, indices {}
			, posted {false}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	MDModelController::MDModelController(QAbstractItemModel& model, QObject* parent)
		: QObject {parent}
		, m {std::make_unique<Internal>(model)}
	{
		connect(&model, &QAbstractItemModel::modelReset, this, &MDModelController::slot_model_reset);
		connect(&model, &QAbstractItemModel::rowsAboutToBeInserted, this, &MDModelController::slot_rows_to_be_inserted);
		connect(&model, &QAbstractItemModel::rowsInserted, this, &MDModelController::slot_rows_inserted);
		connect(&model, &QAbstractItemModel::rowsAboutToBeRemoved, this, &MDModelController::slot_rows_to_be_removed);
		connect(&model, &QAbstractItemModel::rowsRemoved, this, &MDModelController::slot_rows_removed);
		connect(&model, &QAbstractItemModel::rowsAboutToBeMoved, this, &MDModelController::slot_rows_to_be_moved);
		connect(&model, &QAbstractItemModel::rowsMoved, this, &MDModelController::slot_rows_moved);
		connect(&model, &QAbstractItemModel::columnsAboutToBeInserted, this, &MDModelController::slot_columns_to_be_inserted);
		connect(&model, &QAbstractItemModel::columnsInserted, this, &MDModelController::slot_columns_inserted);
		connect(&model, &QAbstractItemModel::columnsAboutToBeRemoved, this, &MDModelController::slot_columns_to_be_removed);
		connect(&model, &QAbstractItemModel::columnsRemoved, this, &MDModelController::slot_columns_removed);
		connect(&model, &QAbstractItemModel::columnsAboutToBeMoved, this, &MDModelController::slot_columns_to_be_moved);
		connect(&model, &QAbstractItemModel::columnsMoved, this, &MDModelController::slot_columns_moved);
	}

	MDModelController::~MDModelController()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Model Slots
	//-----------------------------------------------------------------------------------------------------

	void MDModelController::slot_model_reset()
	{
		request_editors(QModelIndex{}, 0, m->model.rowCount() - 1, 0, m->model.columnCount());
	}

	void MDModelController::slot_rows_to_be_inserted(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_rows_inserted(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_rows_to_be_removed(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_rows_removed(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_rows_to_be_moved(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_row)
	{
		source_parent;
		source_start;
		source_end;
		target_parent;
		target_row;
	}

	void MDModelController::slot_rows_moved(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_row)
	{
		source_parent;
		source_start;
		source_end;
		target_parent;
		target_row;
	}

	void MDModelController::slot_columns_to_be_inserted(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_columns_inserted(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_columns_to_be_removed(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_columns_removed(const QModelIndex& parent, int first, int last)
	{
		parent;
		first;
		last;
	}

	void MDModelController::slot_columns_to_be_moved(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_column)
	{
		source_parent;
		source_start;
		source_end;
		target_parent;
		target_column;
	}

	void MDModelController::slot_columns_moved(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_column)
	{
		source_parent;
		source_start;
		source_end;
		target_parent;
		target_column;
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void MDModelController::request_editors(const QModelIndex& parent, int row_begin, int row_end, int col_begin, int col_end)
	{
		for (int r = row_begin; r <= row_end; ++r)
		{
			for (int c = col_begin; c <= col_end; ++c)
			{
				m->indices.insert(m->model.index(r, c, parent));
			}
		}

		if (!m->posted)
		{
			post_function_event(*this, [this] ()
			{
				QModelIndexList indices;
				indices.reserve((int)m->indices.size());
				std::copy(m->indices.begin(), m->indices.end(), std::back_inserter(indices));
				m->indices.clear();
				m->posted = false;
				emit signal_request_editors(indices);
			});

			m->posted = true;
		}
	}
}