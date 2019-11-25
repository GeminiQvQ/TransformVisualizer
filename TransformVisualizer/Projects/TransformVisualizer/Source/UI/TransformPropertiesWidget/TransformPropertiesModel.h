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
	class TransformPropertiesModel : public QAbstractItemModel
	{
	public:
		// Construction & Destruction
								TransformPropertiesModel	(KernelInterface& kernel_interface, QObject* parent = nullptr);
								~TransformPropertiesModel	();

		// Data
		void					set_transform				(SPtr<Transform> transform);

		// QAbstractItemModel Interface
		virtual QModelIndex		index						(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual QModelIndex		parent						(const QModelIndex& child) const override;
		virtual int				rowCount					(const QModelIndex& parent = QModelIndex()) const override;
		virtual int				columnCount					(const QModelIndex& parent = QModelIndex()) const override;
		virtual Qt::ItemFlags	flags						(const QModelIndex& index) const override;
		virtual QVariant		headerData					(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant		data						(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool			setData						(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	private:
		// Transform Slots
		void					slot_matrix_changed			(const GX::Matrix& matrix);
		void					slot_name_changed			(const QString& name);
		void					slot_enabled_changed		(bool enabled);

		struct Internal;
		const UPtr<Internal> m;
	};
}