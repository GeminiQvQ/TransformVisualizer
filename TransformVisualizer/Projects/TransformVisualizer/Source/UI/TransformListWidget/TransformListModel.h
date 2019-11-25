#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QAbstractItemModel>

// Forward Declarations
namespace GX
{
	class Matrix;
}

namespace TransformVisualizer
{
	class KernelInterface;
	class Transform;
}

namespace TransformVisualizer
{
	class TransformListModel : public QAbstractItemModel
	{
	public:
		// Construction & Destruction
								TransformListModel				(KernelInterface& kernel_interface, QObject* parent = nullptr);
								~TransformListModel				();

		// Index Mapping
		SPtr<Transform>			map_to_transform				(const QModelIndex& index);
		SCPtr<Transform>		map_to_transform				(const QModelIndex& index) const;
		QModelIndex				map_to_index					(const Transform& transform, int column = 0) const;

		// QAbstractItemModel Interface
		virtual QModelIndex		index							(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual QModelIndex		parent							(const QModelIndex& child) const override;
		virtual int				rowCount						(const QModelIndex& parent = QModelIndex()) const override;
		virtual int				columnCount						(const QModelIndex& parent = QModelIndex()) const override;
		virtual Qt::ItemFlags	flags							(const QModelIndex& index) const override;
		virtual QVariant		headerData						(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant		data							(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool			setData							(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		
		virtual QStringList		mimeTypes						() const override;
		virtual QMimeData*		mimeData						(const QModelIndexList& indexes) const override;
		virtual bool			canDropMimeData					(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
		virtual bool			dropMimeData					(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

	private:
		// TransformContainer Slots
		void					slot_to_be_reset				();
		void					slot_reset						();
		void					slot_transform_to_be_added		(SPtr<Transform> transform);
		void					slot_transform_added			(SPtr<Transform> transform);
		void					slot_transform_to_be_removed	(SPtr<Transform> transform);
		void					slot_transform_removed			(SPtr<Transform> transform);
		void					slot_transform_to_be_moved		(SPtr<Transform> transform, int index);
		void					slot_transform_moved			(SPtr<Transform> transform, int index);

		// Transform Slots
		void					slot_name_changed				(const QString& name);
		void					slot_enabled_changed			(bool enabled);

	private:
		// Helpers
		void					connect_transform				(Transform& transform);
		void					connect_transforms				();
		void					disconnect_transform			(Transform& transform);
		void					disconnect_transforms			();

		struct Internal;
		const UPtr<Internal> m;
	};
}