#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QStyledItemDelegate>

namespace GX::QT
{
	class GXQT_API MDViewDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		// Construction & Destruction
							MDViewDelegate			(QObject* parent = nullptr);
							~MDViewDelegate			();

		// QStyledItemDelegate Interface
		virtual QWidget*	createEditor			(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
		virtual void		setEditorData			(QWidget* editor, const QModelIndex& index) const override;
		virtual void		setModelData			(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
		virtual QSize		sizeHint				(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	private:
		// Editor Slots
		void				slot_data_changed		(bool committed);
		void				slot_editor_destroyed	();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}