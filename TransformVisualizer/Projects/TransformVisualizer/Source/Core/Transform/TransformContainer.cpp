// Local Includes
#include <TransformVisualizer/Core/Transform/TransformContainer.h>

// Project Includes
#include <GXLib/Math/Matrix.h>

// Third-Party Includes
#include <QJsonArray>
#include <QJsonObject>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct TransformContainer::Internal
	{
		SPtr<Transform> world_transform;

		Internal()
			: world_transform {std::make_shared<Transform>()}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------
	
	TransformContainer::TransformContainer(GX::QT::CommandController command_controller)
		: GX::QT::ListPtrContainer<Transform> {command_controller}
		, m {std::make_unique<Internal>()}
	{
		m->world_transform->set_name("World Transform");
		m->world_transform->set_read_only(true);

		add_handler(*this);
	}

	TransformContainer::~TransformContainer()
	{
		remove_handler(*this);
	}

	//-----------------------------------------------------------------------------------------------------
	// Serialization
	//-----------------------------------------------------------------------------------------------------

	void TransformContainer::serialize(QJsonObject& json) const
	{
		QJsonArray json_transforms;

		for (const auto& transform : *this)
		{
			QJsonObject json_transform;
			transform->serialize(json_transform);
			json_transforms.push_back(json_transform);
		}

		json["Transforms"] = json_transforms;
	}

	void TransformContainer::deserialize(const QJsonObject& json)
	{
		QVector<SPtr<Transform>> transforms;

		const auto json_transforms = json["Transforms"].toArray();
		for (const auto& json_transform_v : json_transforms)
		{
			auto transform = std::make_shared<Transform>();
			transform->deserialize(json_transform_v.toObject());
			transforms.push_back(transform);
		}

		set(transforms);
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	SPtr<Transform> TransformContainer::world_transform()
	{
		return m->world_transform;
	}

	SCPtr<Transform> TransformContainer::world_transform() const
	{
		return m->world_transform;
	}

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerEventHandler Interface
	//-----------------------------------------------------------------------------------------------------

	void TransformContainer::on_to_be_reset(const QVector<SPtr<Transform>>& /*entries*/)
	{
		emit signal_to_be_reset();
	}

	void TransformContainer::on_reset(const QVector<SPtr<Transform>>& /*entries*/)
	{
		update_world_transform();
		emit signal_reset();
	}

	void TransformContainer::on_entry_to_be_added(SPtr<Transform> entry)
	{
		emit signal_transform_to_be_added(entry);
	}

	void TransformContainer::on_entry_added(SPtr<Transform> entry)
	{
		connect(entry.get(), &Transform::signal_matrix_changed, this, &TransformContainer::update_world_transform);
		connect(entry.get(), &Transform::signal_enabled_changed, this, &TransformContainer::update_world_transform);
		update_world_transform();
		emit signal_transform_added(entry);
	}

	void TransformContainer::on_entry_to_be_removed(SPtr<Transform> entry)
	{
		entry->disconnect(this);
		emit signal_transform_to_be_removed(entry);
	}

	void TransformContainer::on_entry_removed(SPtr<Transform> entry)
	{
		update_world_transform();
		emit signal_transform_removed(entry);
	}

	void TransformContainer::on_entry_to_be_moved(SPtr<Transform> entry, int index)
	{
		emit signal_transform_to_be_moved(entry, index);
	}

	void TransformContainer::on_entry_moved(SPtr<Transform> entry, int index)
	{
		update_world_transform();
		emit signal_transform_moved(entry, index);
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void TransformContainer::update_world_transform()
	{
		auto matrix = GX::Matrix::create_identity();

		for (const auto& transform : *this)
		{
			if (transform->enabled())
			{
				matrix *= transform->matrix();
			}
		}

		m->world_transform->set_matrix(matrix);
	}
}