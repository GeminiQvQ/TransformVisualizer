#pragma once

// Third-Party Includes
#include <QAbstractItemView>
#include <QModelIndex>
#include <QVector>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Selection
	//-----------------------------------------------------------------------------------------------------

	QVector<QModelIndex> selected_indices	(const QAbstractItemView& view);
	QVector<QModelIndex> selected_rows		(const QAbstractItemView& view, int column = 0);
}

// Inline Includes
#include <GXQt/MVC/ViewUtility.inl>