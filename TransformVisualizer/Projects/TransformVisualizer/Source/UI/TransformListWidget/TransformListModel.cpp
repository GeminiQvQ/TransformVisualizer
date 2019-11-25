// Local Includes
#include <TransformVisualizer/UI/TransformListWidget/TransformListModel.h>

// Project Includes
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <TransformVisualizer/Core/Transform/TransformContainer.h>
#include <TransformVisualizer/Utility/CommandUtility.h>
#include <TransformVisualizer/Utility/ModelUtility.h>
#include <TransformVisualizer/KernelInterface.h>

// Third-Party Includes
#include <QDataStream>
#include <QMimeData>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct TransformListModel::Internal
	{
		KernelInterface&	kernel_interface;
		TransformContainer& transform_container;

		Internal(KernelInterface& kernel_interface)
			: kernel_interface {kernel_interface}
			, transform_container {kernel_interface.transform_container()}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	TransformListModel::TransformListModel(KernelInterface& kernel_interface, QObject* parent)
		: QAbstractItemModel {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		connect_transforms();

		connect(&m->transform_container, &TransformContainer::signal_to_be_reset, this, &TransformListModel::slot_to_be_reset);
		connect(&m->transform_container, &TransformContainer::signal_reset, this, &TransformListModel::slot_reset);
		connect(&m->transform_container, &TransformContainer::signal_transform_to_be_added, this, &TransformListModel::slot_transform_to_be_added);
		connect(&m->transform_container, &TransformContainer::signal_transform_added, this, &TransformListModel::slot_transform_added);
		connect(&m->transform_container, &TransformContainer::signal_transform_to_be_removed, this, &TransformListModel::slot_transform_to_be_removed);
		connect(&m->transform_container, &TransformContainer::signal_transform_removed, this, &TransformListModel::slot_transform_removed);
		connect(&m->transform_container, &TransformContainer::signal_transform_to_be_moved, this, &TransformListModel::slot_transform_to_be_moved);
		connect(&m->transform_container, &TransformContainer::signal_transform_moved, this, &TransformListModel::slot_transform_moved);
	}

	TransformListModel::~TransformListModel()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Index Mapping
	//-----------------------------------------------------------------------------------------------------

	SPtr<Transform> TransformListModel::map_to_transform(const QModelIndex& index)
	{
		return std::const_pointer_cast<Transform>(static_cast<const TransformListModel*>(this)->map_to_transform(index));
	}

	SCPtr<Transform> TransformListModel::map_to_transform(const QModelIndex& index) const
	{
		if (index.row() == 1)
		{
			return m->transform_container.world_transform();
		}
		else if (index.row() >= 3)
		{
			return m->transform_container.get(index.row() - 3);
		}
		else
		{
			return nullptr;
		}
	}

	QModelIndex TransformListModel::map_to_index(const Transform& transform, int column) const
	{
		if (&transform == m->transform_container.world_transform().get())
		{
			return index(1, column);
		}
		else
		{
			const int row = m->transform_container.index_of(transform);
			return (row >= 0 ? index(row + 3, column) : QModelIndex{});
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// QAbstractItemModel Interface
	//-----------------------------------------------------------------------------------------------------

	QModelIndex TransformListModel::index(int row, int column, const QModelIndex& /*parent*/) const
	{
		return createIndex(row, column, row);
	}

	QModelIndex TransformListModel::parent(const QModelIndex& /*child*/) const
	{
		return QModelIndex{};
	}

	int TransformListModel::rowCount(const QModelIndex& parent) const
	{
		return (!parent.isValid() ? m->transform_container.size() + 3 : 0);
	}

	int TransformListModel::columnCount(const QModelIndex& /*parent*/) const
	{
		return 1;
	}

	Qt::ItemFlags TransformListModel::flags(const QModelIndex& index) const
	{
		Qt::ItemFlags flags = Qt::ItemIsEnabled;

		if (!index.isValid())
		{
			flags |= Qt::ItemIsDropEnabled;
		}

		if (index.row() == 1)
		{
			flags |= Qt::ItemIsSelectable;
		}

		if (index.row() >= 3)
		{
			flags |= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
		}

		return flags;
	}

	QVariant TransformListModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
			case 0: return "Entries";
			}
		}

		return QVariant{};
	}

	QVariant TransformListModel::data(const QModelIndex& index, int role) const
	{
		if (index.row() == 0)
		{
			return create_section("Combined", index, role);
		}

		if (index.row() == 1)
		{
			if (role == Qt::DisplayRole)
			{
				return m->transform_container.world_transform()->name();
			}
		}

		if (index.row() == 2)
		{
			return create_section("Stack", index, role);
		}

		if (index.row() >= 3)
		{
			const int transform_index = index.row() - 3;

			if (role == Qt::DisplayRole)
			{
				return QString::number(transform_index + 1) + ". " + m->transform_container.get(transform_index)->name();
			}

			if (role == Qt::EditRole)
			{
				return m->transform_container.get(transform_index)->name();
			}

			if (role == Qt::CheckStateRole)
			{
				return (m->transform_container.get(transform_index)->enabled() ? Qt::Checked : Qt::Unchecked);
			}
		}
		
		return QVariant{};
	}

	bool TransformListModel::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		const int transform_index = index.row() - 3;

		if (role == Qt::EditRole)
		{
			auto transform = m->transform_container.get(transform_index);
			exec_property_command(m->kernel_interface.command_controller(), *transform, &Transform::set_name, &Transform::name, value.toString());
			return true;
		}

		if (role == Qt::CheckStateRole)
		{
			auto transform = m->transform_container.get(transform_index);
			exec_property_command(m->kernel_interface.command_controller(), *transform, &Transform::set_enabled, &Transform::enabled, value.toInt() == Qt::Checked);
			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------

	QStringList TransformListModel::mimeTypes() const
	{
		return {"TransformIndices"};
	}

	QMimeData* TransformListModel::mimeData(const QModelIndexList& indexes) const
	{
		QVector<SCPtr<Transform>> transforms;
		for (const auto& index : indexes)
		{
			const auto transform = map_to_transform(index);
			if (transform != nullptr)
			{
				transforms.push_back(transform);
			}
		}

		QByteArray bytes;
		QDataStream stream{&bytes, QIODevice::WriteOnly};
		
		stream << transforms.size();

		for (const auto& transform : transforms)
		{
			stream << m->transform_container.index_of(*transform);
		}

		auto data = new QMimeData{};
		data->setData("TransformIndices", bytes);
		return data;
	}

	bool TransformListModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
	{
		return (QAbstractItemModel::canDropMimeData(data, action, row, column, parent) && row >= 3);
	}

	bool TransformListModel::dropMimeData(const QMimeData* data, Qt::DropAction /*action*/, int row, int /*column*/, const QModelIndex& /*parent*/)
	{
		if (data->hasFormat("TransformIndices"))
		{
			const auto bytes = data->data("TransformIndices");
			QDataStream stream{bytes};

			int num_transforms = 0;
			stream >> num_transforms;

			QVector<SPtr<Transform>> transforms;

			for (int i = 0; i < num_transforms; ++i)
			{
				int transform_index = 0;
				stream >> transform_index;
				transforms.push_back(m->transform_container.get(transform_index));
			}

			m->transform_container.move(transforms, row - 3);

			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// TransformContainer Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformListModel::slot_to_be_reset()
	{
		disconnect_transforms();

		beginResetModel();
	}

	void TransformListModel::slot_reset()
	{
		connect_transforms();

		endResetModel();
	}

	void TransformListModel::slot_transform_to_be_added(SPtr<Transform> transform)
	{
		const int index = rowCount();
		beginInsertRows(QModelIndex{}, rowCount(), rowCount());
	}

	void TransformListModel::slot_transform_added(SPtr<Transform> transform)
	{
		connect_transform(*transform);

		endInsertRows();
	}

	void TransformListModel::slot_transform_to_be_removed(SPtr<Transform> transform)
	{
		disconnect_transform(*transform);

		const int index = m->transform_container.index_of(*transform) + 3;
		beginRemoveRows(QModelIndex{}, index, index);
	}

	void TransformListModel::slot_transform_removed(SPtr<Transform> /*transform*/)
	{
		endRemoveRows();
	}

	void TransformListModel::slot_transform_to_be_moved(SPtr<Transform> transform, int index)
	{
		const int index_old = m->transform_container.index_of(*transform) + 3;
		beginMoveRows(QModelIndex{}, index_old, index_old, QModelIndex{}, index + 3);
	}

	void TransformListModel::slot_transform_moved(SPtr<Transform> /*transform*/, int /*index*/)
	{
		endMoveRows();
	}

	//-----------------------------------------------------------------------------------------------------
	// Transform Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformListModel::slot_name_changed(const QString& /*name*/)
	{
		const auto transform = static_cast<const Transform*>(sender());
		const auto row = m->transform_container.index_of(*transform) + 3;
		const auto index = this->index(row, 0);
		emit dataChanged(index, index, {Qt::DisplayRole});
	}

	void TransformListModel::slot_enabled_changed(bool /*enabled*/)
	{
		const auto transform = static_cast<const Transform*>(sender());
		const auto row = m->transform_container.index_of(*transform) + 3;
		const auto index = this->index(row, 0);
		emit dataChanged(index, index, {Qt::CheckStateRole});
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void TransformListModel::connect_transform(Transform& transform)
	{
		connect(&transform, &Transform::signal_name_changed, this, &TransformListModel::slot_name_changed);
		connect(&transform, &Transform::signal_enabled_changed, this, &TransformListModel::slot_enabled_changed);
	}

	void TransformListModel::connect_transforms()
	{
		for (const auto& transform : m->transform_container)
		{
			connect_transform(*transform);
		}
	}

	void TransformListModel::disconnect_transform(Transform& transform)
	{
		transform.disconnect(this);
	}

	void TransformListModel::disconnect_transforms()
	{
		for (const auto& transform : m->transform_container)
		{
			disconnect_transform(*transform);
		}
	}
}