#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QAbstractItemModel>

// Forward Declarations
namespace TransformVisualizer
{
	class KernelInterface;
}

namespace TransformVisualizer
{
	class SettingsModel : public QAbstractItemModel
	{
	public:
		// Construction & Destruction
								SettingsModel		(KernelInterface& kernel_interface, QObject* parent = nullptr);
								~SettingsModel		();

		// QAbstractItemModel Interface
		virtual QModelIndex		index				(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual QModelIndex		parent				(const QModelIndex& child) const override;
		virtual int				rowCount			(const QModelIndex& parent = QModelIndex()) const override;
		virtual int				columnCount			(const QModelIndex& parent = QModelIndex()) const override;
		virtual Qt::ItemFlags	flags				(const QModelIndex& index) const override;
		virtual QVariant		headerData			(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant		data				(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool			setData				(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	private:
		// State Slots
		void					slot_camera_changed	();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}