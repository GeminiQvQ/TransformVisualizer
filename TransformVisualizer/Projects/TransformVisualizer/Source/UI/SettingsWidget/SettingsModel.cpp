// Local Includes
#include <TransformVisualizer/UI/SettingsWidget/SettingsModel.h>

// Project Includes
#include <GXLib/Graphics/Camera.h>
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <TransformVisualizer/Core/Settings.h>
#include <TransformVisualizer/Core/State.h>
#include <TransformVisualizer/Utility/ModelUtility.h>
#include <TransformVisualizer/KernelInterface.h>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct SettingsModel::Internal
	{
		KernelInterface& kernel_interface;

		Internal(KernelInterface& kernel_interface)
			: kernel_interface {kernel_interface}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	SettingsModel::SettingsModel(KernelInterface& kernel_interface, QObject* parent)
		: QAbstractItemModel {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		connect(&kernel_interface.state(), &State::signal_camera_change_end, this, &SettingsModel::slot_camera_changed);
	}

	SettingsModel::~SettingsModel()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// QAbstractItemModel Interface
	//-----------------------------------------------------------------------------------------------------

	QModelIndex SettingsModel::index(int row, int column, const QModelIndex& /*parent*/) const
	{
		return createIndex(row, column, row);
	}

	QModelIndex SettingsModel::parent(const QModelIndex& /*child*/) const
	{
		return QModelIndex{};
	}

	int SettingsModel::rowCount(const QModelIndex& parent) const
	{
		return (!parent.isValid() ? 16 : 0);
	}

	int SettingsModel::columnCount(const QModelIndex& /*parent*/) const
	{
		return 2;
	}

	Qt::ItemFlags SettingsModel::flags(const QModelIndex& /*index*/) const
	{
		return Qt::ItemIsEnabled;
	}

	QVariant SettingsModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
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

	QVariant SettingsModel::data(const QModelIndex& index, int role) const
	{
		const auto& settings = m->kernel_interface.settings();
		const auto& camera = m->kernel_interface.state().camera();

		auto get_color_data = [&index, role] (const QString& name, const GX::Vector4F& color) -> QVariant
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return name;
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData{GX::QT::create_model_data_color_button(color)});
			}

			return QVariant();
		};

		// Section: Colors.
		if (index.row() == 0)
		{
			return create_section("Colors", index, role);
		}

		// Colors.
		switch (index.row())
		{
		case 1: return get_color_data("Background Color", settings.background_color);
		case 2: return get_color_data("Cube Color (Front)", settings.cube_color[0]);
		case 3: return get_color_data("Cube Color (Back)", settings.cube_color[1]);
		case 4: return get_color_data("Cube Color (Left)", settings.cube_color[2]);
		case 5: return get_color_data("Cube Color (Right)", settings.cube_color[3]);
		case 6: return get_color_data("Cube Color (Top)", settings.cube_color[4]);
		case 7: return get_color_data("Cube Color (Bottom)", settings.cube_color[5]);
		}
		
		// Section: Projection.
		if (index.row() == 8)
		{
			return create_section("Projection", index, role);
		}

		// FoV.
		if (index.row() == 9)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "FoV";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData{GX::QT::create_model_data_spinbox(settings.projection_fov, 0.0f, 180.0f)});
			}
		}

		// Near plane.
		if (index.row() == 10)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Near Plane";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData{GX::QT::create_model_data_spinbox(settings.projection_near_plane)});
			}
		}

		// Far plane.
		if (index.row() == 11)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Far Plane";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData{GX::QT::create_model_data_spinbox(settings.projection_far_plane)});
			}
		}

		// Section: Camera.
		if (index.row() == 12)
		{
			return create_section("Camera", index, role);
		}

		// Position.
		if (index.row() == 13)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Position";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData
				{
					GX::QT::create_model_data_spinbox(camera.position().x),
					GX::QT::create_model_data_spinbox(camera.position().y),
					GX::QT::create_model_data_spinbox(camera.position().z)
				});
			}
		}

		// Target.
		if (index.row() == 14)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Target";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData
				{
					GX::QT::create_model_data_spinbox(camera.target().x),
					GX::QT::create_model_data_spinbox(camera.target().y),
					GX::QT::create_model_data_spinbox(camera.target().z)
				});
			}
		}

		// Up.
		if (index.row() == 15)
		{
			if (index.column() == 0 && role == Qt::DisplayRole)
			{
				return "Up";
			}

			if (index.column() == 1 && role == GX::QT::ModelDataRole)
			{
				return QVariant::fromValue(GX::QT::ModelData
				{
					GX::QT::create_model_data_spinbox(camera.up().x),
					GX::QT::create_model_data_spinbox(camera.up().y),
					GX::QT::create_model_data_spinbox(camera.up().z)
				});
			}
		}

		return QVariant{};
	}

	bool SettingsModel::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		auto& settings = m->kernel_interface.settings();

		if (index.column() == 1 && role == GX::QT::ModelDataRole)
		{
			const auto& data = value.value<GX::QT::ModelData>();

			if (!data.is_committed())
			{
				return false;
			}

			auto set_color_data = [&data] (GX::Vector4F& color)
			{
				color = data[0].value().value<GX::Vector4F>();
			};

			// Colors.
			switch (index.row())
			{
			case 1: set_color_data(settings.background_color); break;
			case 2: set_color_data(settings.cube_color[0]); break;
			case 3: set_color_data(settings.cube_color[1]); break;
			case 4: set_color_data(settings.cube_color[2]); break;
			case 5: set_color_data(settings.cube_color[3]); break;
			case 6: set_color_data(settings.cube_color[4]); break;
			case 7: set_color_data(settings.cube_color[5]); break;
			}

			// FoV.
			if (index.row() == 9)
			{
				settings.projection_fov = data[0].value().toFloat();
			}

			// Near plane.
			if (index.row() == 10)
			{
				settings.projection_near_plane = data[0].value().toFloat();
			}

			// Far plane.
			if (index.row() == 11)
			{
				settings.projection_far_plane = data[0].value().toFloat();
			}

			// Camera position.
			if (index.row() == 13)
			{
				m->kernel_interface.state().begin_camera_change();
				auto& camera = m->kernel_interface.state().camera();
				const auto position = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
				camera.set_position(position);
				m->kernel_interface.state().end_camera_change();
			}

			// Camera position.
			if (index.row() == 14)
			{
				m->kernel_interface.state().begin_camera_change();
				auto& camera = m->kernel_interface.state().camera();
				const auto target = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
				camera.set_target(target);
				m->kernel_interface.state().end_camera_change();
			}

			// Camera position.
			if (index.row() == 15)
			{
				m->kernel_interface.state().begin_camera_change();
				auto& camera = m->kernel_interface.state().camera();
				const auto up = GX::Vector3F{data[0].value().toFloat(), data[1].value().toFloat(), data[2].value().toFloat()};
				camera.set_up(up);
				m->kernel_interface.state().end_camera_change();
			}

			emit dataChanged(index, index, {GX::QT::ModelDataRole});

			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// State Slots
	//-----------------------------------------------------------------------------------------------------

	void SettingsModel::slot_camera_changed()
	{
		emit dataChanged(index(13, 1), index(15, 1), {GX::QT::ModelDataRole});
	}
}