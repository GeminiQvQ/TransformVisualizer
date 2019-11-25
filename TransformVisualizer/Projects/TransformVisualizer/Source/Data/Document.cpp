// Local Includes
#include <TransformVisualizer/Data/Document.h>

// Project Includes
#include <GXLib/Math/Matrix.h>
#include <TransformVisualizer/Core/Transform/TransformContainer.h>
#include <TransformVisualizer/Core/State.h>

// Third-Party Includes
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct Document::Internal
	{
		State					state;
		GX::QT::CommandStack	command_stack;
		TransformContainer		transform_container;
		QString					filename;

		Internal()
			: state {}
			, command_stack {}
			, transform_container {GX::QT::CommandController{command_stack}}
			, filename {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Document::Document()
		: m {std::make_unique<Internal>()}
	{
		clear();
	}

	Document::~Document()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Save & Load
	//-----------------------------------------------------------------------------------------------------

	bool Document::save(const QString& filename)
	{
		// Serialize.
		QJsonObject json_transform_container;
		m->transform_container.serialize(json_transform_container);

		QJsonObject json;
		json["TransformContainer"] = json_transform_container;

		// Write to file.
		QFile file{filename};
		if (!file.open(QIODevice::WriteOnly))
		{
			return false;
		}

		file.write(QJsonDocument{json}.toJson(QJsonDocument::Indented));
		file.close();

		m->command_stack.setClean();
		m->filename = filename;

		return true;
	}

	bool Document::load(const QString& filename)
	{
		// Read from file.
		QFile file {filename};
		if (!file.open(QIODevice::ReadOnly))
		{
			return false;
		}

		// De-serialize.
		auto json_doc = QJsonDocument::fromJson(file.readAll());
		if (json_doc.isNull())
		{
			return false;
		}

		auto json = json_doc.object();
		
		auto command_controller = GX::QT::CommandController{m->command_stack};
		command_controller.set_enabled(false);

		m->transform_container.deserialize(json["TransformContainer"].toObject());

		command_controller.set_enabled(true);

		m->command_stack.clear();
		m->filename = filename;
		m->state.select_transforms(QVector<SPtr<Transform>>{});

		return true;
	}

	void Document::clear()
	{
		// Reset.
		m->transform_container.clear();
		m->command_stack.clear();
		m->filename.clear();
		m->state.select_transforms(QVector<SPtr<Transform>>{});

		// Set up default transforms.
		auto command_controller = GX::QT::CommandController{m->command_stack};
		command_controller.set_enabled(false);

		auto transform_translate = std::make_shared<Transform>();
		transform_translate->set_name("Translation");
		transform_translate->set_matrix(GX::Matrix::create_identity());
		m->transform_container.add(transform_translate);

		auto transform_rotate = std::make_shared<Transform>();
		transform_rotate->set_name("Rotation");
		transform_rotate->set_matrix(GX::Matrix::create_identity());
		m->transform_container.add(transform_rotate);

		auto transform_scale = std::make_shared<Transform>();
		transform_scale->set_name("Scale");
		transform_scale->set_matrix(GX::Matrix::create_identity());
		m->transform_container.add(transform_scale);

		command_controller.set_enabled(true);
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	const QString& Document::filename() const
	{
		return m->filename;
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	State& Document::state()
	{
		return m->state;
	}

	const State& Document::state() const
	{
		return m->state;
	}

	GX::QT::CommandStack& Document::command_stack()
	{
		return m->command_stack;
	}

	const GX::QT::CommandStack& Document::command_stack() const
	{
		return m->command_stack;
	}

	TransformContainer& Document::transform_container()
	{
		return m->transform_container;
	}

	const TransformContainer& Document::transform_container() const
	{
		return m->transform_container;
	}
}