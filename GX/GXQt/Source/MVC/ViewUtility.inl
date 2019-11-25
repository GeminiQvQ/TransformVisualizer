// Third-Party Includes
#include <QItemSelectionModel>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Selection
	//-----------------------------------------------------------------------------------------------------

	inline QVector<QModelIndex> selected_indices(const QAbstractItemView& view)
	{
		QVector<QModelIndex> indices;

		for (auto&& selection : view.selectionModel()->selection())
		{
			for (int r = selection.top(); r <= selection.bottom(); ++r)
			{
				for (int c = selection.left(); c <= selection.right(); ++c)
				{
					indices.push_back(selection.model()->index(r, c, selection.parent()));
				}
			}
		}

		return indices;
	}

	inline QVector<QModelIndex> selected_rows(const QAbstractItemView& view, int column)
	{
		QVector<QModelIndex> indices;

		for (auto&& selection : view.selectionModel()->selection())
		{
			for (int r = selection.top(); r <= selection.bottom(); ++r)
			{
				indices.push_back(selection.model()->index(r, column, selection.parent()));
			}
		}

		return indices;
	}
}