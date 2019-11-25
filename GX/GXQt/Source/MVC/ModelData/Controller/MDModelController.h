#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QAbstractItemModel>
#include <QObject>

namespace GX::QT
{
	class MDModelController : public QObject
	{
		Q_OBJECT

	public:
		// Construction & Destruction
				MDModelController			(QAbstractItemModel& model, QObject* parent = nullptr);
				~MDModelController			();

	signals:
		// Signals
		void	signal_request_editors		(const QModelIndexList& indices);

	private:
		// Model Slots
		void	slot_model_reset			();
		void	slot_rows_to_be_inserted	(const QModelIndex& parent, int first, int last);
		void	slot_rows_inserted			(const QModelIndex& parent, int first, int last);
		void	slot_rows_to_be_removed		(const QModelIndex& parent, int first, int last);
		void	slot_rows_removed			(const QModelIndex& parent, int first, int last);
		void	slot_rows_to_be_moved		(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_row);
		void	slot_rows_moved				(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_row);
		void	slot_columns_to_be_inserted	(const QModelIndex& parent, int first, int last);
		void	slot_columns_inserted		(const QModelIndex& parent, int first, int last);
		void	slot_columns_to_be_removed	(const QModelIndex& parent, int first, int last);
		void	slot_columns_removed		(const QModelIndex& parent, int first, int last);
		void	slot_columns_to_be_moved	(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_column);
		void	slot_columns_moved			(const QModelIndex& source_parent, int source_start, int source_end, const QModelIndex& target_parent, int target_column);

	private:
		// Helpers
		void	request_editors				(const QModelIndex& parent, int row_begin, int row_end, int col_begin, int col_end);

		struct Internal;
		const UPtr<Internal> m;
	};
}