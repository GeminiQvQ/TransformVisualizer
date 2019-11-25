// Local Includes
#include <TransformVisualizer/Core/Transform/Transform.h>

// Project Includes
#include <GXLib/Math/Matrix.h>
#include <GXQt/Utility/JSON.h>

// Third-Party Includes
#include <QJsonObject>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct Transform::Internal
	{
		QString		name;
		GX::Matrix	matrix;
		bool		enabled;
		bool		read_only;

		Internal()
			: name {"New Transform"}
			, matrix {GX::Matrix::create_identity()}
			, enabled {true}
			, read_only {false}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Transform::Transform()
		: m {std::make_unique<Internal>()}
	{
	}

	Transform::~Transform()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void Transform::set_name(const QString& name)
	{
		m->name = name;
		emit signal_name_changed(name);
	}

	const QString& Transform::name() const
	{
		return m->name;
	}

	//-----------------------------------------------------------------------------------------------------

	void Transform::set_matrix(const GX::Matrix& matrix)
	{
		m->matrix = matrix;
		emit signal_matrix_changed(matrix);
	}

	const GX::Matrix& Transform::matrix() const
	{
		return m->matrix;
	}

	//-----------------------------------------------------------------------------------------------------

	void Transform::set_enabled(bool enabled)
	{
		m->enabled = enabled;
		emit signal_enabled_changed(enabled);
	}

	bool Transform::enabled() const
	{
		return m->enabled;
	}

	//-----------------------------------------------------------------------------------------------------

	void Transform::set_read_only(bool read_only)
	{
		m->read_only = read_only;
		emit signal_read_only_changed(read_only);
	}

	bool Transform::read_only() const
	{
		return m->read_only;
	}

	//-----------------------------------------------------------------------------------------------------
	// Serialization
	//-----------------------------------------------------------------------------------------------------

	void Transform::serialize(QJsonObject& json) const
	{
		json["Enabled"] = m->enabled;
		json["Name"] = m->name;
		json["ReadOnly"] = m->read_only;
		
		QJsonArray json_values;
		GX::QT::to_json_array(json_values, m->matrix.data().begin(), m->matrix.data().end());
		json["Values"] = json_values;
	}

	void Transform::deserialize(const QJsonObject& json)
	{
		m->enabled = json["Enabled"].toBool(m->enabled);
		m->name = json["Name"].toString(m->name);
		m->read_only = json["ReadOnly"].toBool(m->read_only);

		GX::QT::from_json_array<float>(json["Values"].toArray(), m->matrix.data().begin());
	}
}