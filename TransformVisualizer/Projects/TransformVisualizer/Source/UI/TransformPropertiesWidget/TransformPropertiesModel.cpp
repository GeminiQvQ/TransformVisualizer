// Local Includes
#include <TransformVisualizer/UI/TransformPropertiesWidget/TransformPropertiesModel.h>

// Project Includes
#include <GXLib/Math/Matrix.h>
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <TransformVisualizer/Core/Transform/Transform.h>
#include <TransformVisualizer/Utility/CommandUtility.h>
#include <TransformVisualizer/Utility/ModelUtility.h>
#include <TransformVisualizer/KernelInterface.h>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct TransformPropertiesModel::Internal
	{
		KernelInterface&	kernel_interface;

		SPtr<Transform>		transform;

		GX::Vector3F		translation;
		GX::Vector3F		rotation;
		GX::Vector3F		scale;
		bool				use_components;

		Internal(KernelInterface& kernel_interface)
			: kernel_interface {kernel_interface}
			, transform {}
			, translation {}
			, rotation {}
			, scale {}
			, use_components {false}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	TransformPropertiesModel::TransformPropertiesModel(KernelInterface& kernel_interface, QObject* parent)
		: QAbstractItemModel {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
	}

	TransformPropertiesModel::~TransformPropertiesModel()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Data
	//-----------------------------------------------------------------------------------------------------

	void TransformPropertiesModel::set_transform(SPtr<Transform> transform)
	{
		if (m->transform != nullptr)
		{
			m->translation = GX::Vector3F{};
			m->rotation = GX::Vector3F{};
			m->scale = GX::Vector3F{};
			m->use_components = false;

			m->transform->disconnect(this);
		}

		m->transform = transform;

		if (m->transform != nullptr)
		{
			m->translation = GX::extract_translation(m->transform->matrix());
			m->rotation = GX::extract_rotation(m->transform->matrix());
			m->scale = GX::extract_scale(m->transform->matrix());

			connect(m->transform.get(), &Transform::signal_matrix_changed, this, &TransformPropertiesModel::slot_matrix_changed);
			connect(m->transform.get(), &Transform::signal_name_changed, this, &TransformPropertiesModel::slot_name_changed);
			connect(m->transform.get(), &Transform::signal_enabled_changed, this, &TransformPropertiesModel::slot_enabled_changed);
		}

		emit dataChanged(index(0, 1), index(rowCount() - 1, 1));
	}

	//-----------------------------------------------------------------------------------------------------
	// QAbstractItemModel Interface
	//-----------------------------------------------------------------------------------------------------

	QModelIndex TransformPropertiesModel::index(int row, int column, const QModelIndex& /*parent*/) const
	{
		return createIndex(row, column, row);
	}

	QModelIndex TransformPropertiesModel::parent(const QModelIndex& /*child*/) const
	{
		return QModelIndex{};
	}

	int TransformPropertiesModel::rowCount(const QModelIndex& parent) const
	{
		return (!parent.isValid() ? 9 : 0);
	}

	int TransformPropertiesModel::columnCount(const QModelIndex& /*parent*/) const
	{
		return 2;
	}

	Qt::ItemFlags TransformPropertiesModel::flags(const QModelIndex& /*index*/) const
	{
		return Qt::ItemIsEnabled;
	}

	QVariant TransformPropertiesModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
			case 0: return "Key";
			case 1: return "Value";
			}
		}

		return QVariant{};
	}

	QVariant TransformPropertiesModel::data(const QModelIndex& index, int role) const
	{
		GX::QT::ModelData data;

		// Section: Matrix.
		if (index.row() == 0)
		{
			return create_section("Matrix", index, role);
		}

		// Matrix values.
		if (index.row() == 1)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Values";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				data.reserve(16);

				for (int r = 0; r < 4; ++r)
				{
					for (int c = 0; c < 4; ++c)
					{
						const float value = (m->transform != nullptr ? m->transform->matrix()(r, c) : 0.0f);
						auto entry = GX::QT::create_model_data_spinbox(value);
						entry.set_editor_pos(c, r);
						data.push_back(entry);
					}
				}
			}
		}

		// Section: Components.
		if (index.row() == 2)
		{
			return create_section("Components", index, role);
		}

		// Translate.
		if (index.row() == 3)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Translation";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				data.push_back(GX::QT::create_model_data_spinbox(m->translation.x));
				data.push_back(GX::QT::create_model_data_spinbox(m->translation.y));
				data.push_back(GX::QT::create_model_data_spinbox(m->translation.z));
			}
		}

		// Rotate.
		if (index.row() == 4)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Rotation";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				data.push_back(GX::QT::create_model_data_spinbox(m->rotation.x));
				data.push_back(GX::QT::create_model_data_spinbox(m->rotation.y));
				data.push_back(GX::QT::create_model_data_spinbox(m->rotation.z));
			}
		}

		// Scale.
		if (index.row() == 5)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Scale";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				data.push_back(GX::QT::create_model_data_spinbox(m->scale.x));
				data.push_back(GX::QT::create_model_data_spinbox(m->scale.y));
				data.push_back(GX::QT::create_model_data_spinbox(m->scale.z));
			}
		}

		// Section: Properties.
		if (index.row() == 6)
		{
			return create_section("Properties", index, role);
		}

		// Name.
		if (index.row() == 7)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Name";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				const auto value = (m->transform != nullptr ? m->transform->name() : QString{});
				data.push_back(GX::QT::create_model_data_lineedit(value));
			}
		}

		// Enabled.
		if (index.row() == 8)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Enabled";
			}
			
			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				const bool value = (m->transform != nullptr ? m->transform->enabled() : false);
				data.push_back(GX::QT::create_model_data_checkbox(value));
			}
		}

		if (role == GX::QT::ModelDataRole && !data.isEmpty())
		{
			for (auto& entry : data)
			{
				entry.set_attribute(GX::QT::ModelDataAttribute::Enabled, m->transform != nullptr);
				entry.set_attribute(GX::QT::ModelDataAttribute::ReadOnly, (m->transform != nullptr && m->transform->read_only()));
			}

			return QVariant::fromValue(data);
		}
		
		return QVariant{};
	}

	bool TransformPropertiesModel::setData(const QModelIndex& index, const QVariant& value, int /*role*/)
	{
		const auto data = value.value<GX::QT::ModelData>();
		
		if (!data.is_committed())
		{
			return false;
		}

		// Matrix values.
		if (index.row() == 1)
		{
			GX::Matrix matrix;

			for (int i = 0; i < 16; ++i)
			{
				matrix[i] = data[i].value().toFloat();
			}

			m->use_components = false;

			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_matrix, &Transform::matrix, matrix);

			return true;
		}

		// Translation.
		if (index.row() == 3)
		{
			m->translation = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
			m->use_components = true;

			const auto matrix = GX::Matrix::create_translation(m->translation)
							  * GX::Matrix::create_rotation(m->rotation)
							  * GX::Matrix::create_scale(m->scale);

			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_matrix, &Transform::matrix, matrix);

			return true;
		}

		// Rotation.
		if (index.row() == 4)
		{
			m->rotation = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
			m->use_components = true;

			const auto matrix = GX::Matrix::create_translation(m->translation)
							  * GX::Matrix::create_rotation(m->rotation)
							  * GX::Matrix::create_scale(m->scale);

			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_matrix, &Transform::matrix, matrix);

			return true;
		}

		// Scale.
		if (index.row() == 5)
		{
			m->scale = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
			m->use_components = true;

			const auto matrix = GX::Matrix::create_translation(m->translation)
							  * GX::Matrix::create_rotation(m->rotation)
							  * GX::Matrix::create_scale(m->scale);

			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_matrix, &Transform::matrix, matrix);

			return true;
		}

		// Name.
		if (index.row() == 7)
		{
			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_name, &Transform::name, data[0].value().toString());
		}

		// Enabled.
		if (index.row() == 8)
		{
			exec_property_command(m->kernel_interface.command_controller(), *m->transform, &Transform::set_enabled, &Transform::enabled, data[0].value().toBool());
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// Transform Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformPropertiesModel::slot_matrix_changed(const GX::Matrix& /*matrix*/)
	{
		if (!m->use_components)
		{
			m->translation = GX::extract_translation(m->transform->matrix());
			m->rotation = GX::extract_rotation(m->transform->matrix());
			m->scale = GX::extract_scale(m->transform->matrix());
		}

		emit dataChanged(index(0, 1), index(4, 1), {GX::QT::ModelDataRole});
	}

	void TransformPropertiesModel::slot_name_changed(const QString& /*name*/)
	{
		const auto index = this->index(7, 1);
		emit dataChanged(index, index, {GX::QT::ModelDataRole});
	}

	void TransformPropertiesModel::slot_enabled_changed(bool /*enabled*/)
	{
		const auto index = this->index(8, 1);
		emit dataChanged(index, index, {GX::QT::ModelDataRole});
	}
}