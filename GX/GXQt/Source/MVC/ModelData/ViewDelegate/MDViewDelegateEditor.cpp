// Local Includes
#include <GXQt/MVC/ModelData/ViewDelegate/MDViewDelegateEditor.h>

// Project Includes
#include <GXLib/Math/Vector.h>
#include <GXQt/Utility/Layout.h>
#include <GXQt/Widget/ColorButtonWidget.h>

// Third-Party Includes
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Interface
	//-----------------------------------------------------------------------------------------------------

	MDViewDelegateEditor::MDViewDelegateEditor(QLayout* layout, QWidget* parent)
		: QWidget {parent}
	{
		layout->setContentsMargins(1, 1, 1, 1);
		layout->setSpacing(1);
		setLayout(layout);
	}

	void MDViewDelegateEditor::data_changed()
	{
		emit signal_data_changed(false);
	}

	void MDViewDelegateEditor::data_committed()
	{
		ModelDataEntry entry;
		get_data(entry);

		if (entry.value() != m_value_cached)
		{
			m_value_cached = entry.value();
			emit signal_data_changed(true);
		}
	}

	void MDViewDelegateEditor::cache_data()
	{
		ModelDataEntry entry;
		get_data(entry);
		m_value_cached = entry.value();
	}

	//-----------------------------------------------------------------------------------------------------
	// Model Data Bundle
	//-----------------------------------------------------------------------------------------------------
	
	struct MDEModelData::Internal
	{
		ModelData						data;
		QVector<MDViewDelegateEditor*>	editors;
	};

	//-----------------------------------------------------------------------------------------------------

	MDEModelData::MDEModelData(QWidget* parent)
		: MDViewDelegateEditor {new QGridLayout{}, parent}
		, m {std::make_unique<Internal>()}
	{
	}

	MDEModelData::~MDEModelData()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	
	void MDEModelData::create_editors(const ModelData& data)
	{
		if (data.size() != m->editors.size())
		{
			for (auto editor : m->editors)
			{
				editor->setParent(nullptr);
				editor->setVisible(false);
				editor->deleteLater();
			}

			while (layout()->count() > 0)
			{
				layout()->removeItem(layout()->itemAt(layout()->count() - 1));
			}

			m->editors.clear();

			auto editor_layout = static_cast<QGridLayout*>(layout());

			for (const auto& entry : data)
			{
				auto editor = data.editor_creator()(entry);
				editor->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
				connect(editor, &MDViewDelegateEditor::signal_data_changed, this, &MDEModelData::slot_data_changed);
				m->editors.push_back(editor);

				auto editor_pos = entry.editor_pos();
				int editor_pos_x = (editor_pos.x() > -1 ? editor_pos.x() : num_col_items(*editor_layout, editor_pos.y() > -1 ? editor_pos.y() : 0));
				int editor_pos_y = (editor_pos.y() > -1 ? editor_pos.y() : num_row_items(*editor_layout, editor_pos_x));
				editor_layout->addWidget(editor, editor_pos_y, editor_pos_x);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEModelData::set_data(const ModelData& data)
	{
		m->data = data;

		create_editors(data);

		for (int i = 0; i < data.size(); ++i)
		{
			auto& editor = m->editors[i];

			editor->setEnabled(data[i].attribute(ModelDataAttribute::Enabled, true).toBool());
			editor->setVisible(data[i].attribute(ModelDataAttribute::Visible, true).toBool());

			editor->blockSignals(true);
			editor->set_data(data[i]);
			editor->blockSignals(false);
		}
	}

	const ModelData& MDEModelData::get_data() const
	{
		return m->data;
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEModelData::set_data(const ModelDataEntry& entry)
	{
		set_data(entry.value().value<ModelData>());
	}

	void MDEModelData::get_data(ModelDataEntry& entry) const
	{
		entry.set_value(QVariant::fromValue(m->data));
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEModelData::slot_data_changed(bool committed)
	{
		auto editor = static_cast<MDViewDelegateEditor*>(sender());
		auto it = std::find(m->editors.begin(), m->editors.end(), editor);
		auto index = std::distance(m->editors.begin(), it);
		editor->get_data(m->data[index]);
		m->data.set_committed(committed);
		emit signal_data_changed(committed);
	}

	//-----------------------------------------------------------------------------------------------------
	// Bool
	//-----------------------------------------------------------------------------------------------------

	struct MDEBool::Internal
	{
		QCheckBox* checkbox;

		Internal()
			: checkbox {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------

	MDEBool::MDEBool()
		: m {std::make_unique<Internal>()}
	{
		m->checkbox = new QCheckBox{};
		connect(m->checkbox, &QCheckBox::toggled, this, &MDEBool::data_committed);
		layout()->addWidget(m->checkbox);
	}

	MDEBool::~MDEBool()
	{
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEBool::set_data(const ModelDataEntry& entry)
	{
		m->checkbox->setChecked(entry.value().toBool());
		m->checkbox->setEnabled(!entry.attribute(ModelDataAttribute::ReadOnly, false).toBool());
		cache_data();
	}

	void MDEBool::get_data(ModelDataEntry& entry) const
	{
		entry.set_value(m->checkbox->isChecked());
	}

	//-----------------------------------------------------------------------------------------------------
	// Float
	//-----------------------------------------------------------------------------------------------------

	struct MDEFloat::Internal
	{
		QDoubleSpinBox* spinbox;

		Internal()
			: spinbox {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------

	MDEFloat::MDEFloat()
		: m {std::make_unique<Internal>()}
	{
		m->spinbox = new QDoubleSpinBox();

		connect(m->spinbox, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, &MDEFloat::data_changed);
		connect(m->spinbox, &QDoubleSpinBox::editingFinished, this, &MDEFloat::data_committed);

		layout()->addWidget(m->spinbox);
	}

	MDEFloat::~MDEFloat()
	{
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEFloat::set_data(const ModelDataEntry& entry)
	{
		m->spinbox->setValue(entry.value().toDouble());
		m->spinbox->setReadOnly(entry.attribute(ModelDataAttribute::ReadOnly, false).toBool());
		m->spinbox->setMinimum(entry.attribute(ModelDataAttribute::ValueMin, m->spinbox->minimum()).toDouble());
		m->spinbox->setMaximum(entry.attribute(ModelDataAttribute::ValueMax, m->spinbox->maximum()).toDouble());
		cache_data();
	}

	void MDEFloat::get_data(ModelDataEntry& entry) const
	{
		entry.set_value(m->spinbox->value());
	}

	//-----------------------------------------------------------------------------------------------------
	// String
	//-----------------------------------------------------------------------------------------------------

	struct MDEString::Internal
	{
		QLineEdit* lineedit;

		Internal()
			: lineedit {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------

	MDEString::MDEString()
		: m {std::make_unique<Internal>()}
	{
		m->lineedit = new QLineEdit();

		connect(m->lineedit, &QLineEdit::textChanged, this, &MDEString::data_changed);
		connect(m->lineedit, &QLineEdit::editingFinished, this, &MDEString::data_committed);

		layout()->addWidget(m->lineedit);
	}

	MDEString::~MDEString()
	{
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEString::set_data(const ModelDataEntry& entry)
	{
		m->lineedit->setText(entry.value().toString());
		m->lineedit->setReadOnly(entry.attribute(ModelDataAttribute::ReadOnly, false).toBool());
		cache_data();
	}

	void MDEString::get_data(ModelDataEntry& entry) const
	{
		entry.set_value(m->lineedit->text());
	}

	//-----------------------------------------------------------------------------------------------------
	// Vector4F (Color)
	//-----------------------------------------------------------------------------------------------------

	struct MDEVector4FColor::Internal
	{
		ColorButtonWidget* color_button;

		Internal()
			: color_button {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------

	MDEVector4FColor::MDEVector4FColor()
		: m {std::make_unique<Internal>()}
	{
		m->color_button = new ColorButtonWidget{};

		connect(m->color_button, &ColorButtonWidget::signal_color_changed, this, &MDEVector4FColor::data_changed);
		connect(m->color_button, &ColorButtonWidget::signal_color_committed, this, &MDEVector4FColor::data_committed);

		layout()->addWidget(m->color_button);
	}

	MDEVector4FColor::~MDEVector4FColor()
	{
	}

	//-----------------------------------------------------------------------------------------------------

	void MDEVector4FColor::set_data(const ModelDataEntry& entry)
	{
		auto color = entry.value().value<Vector4F>();
		m->color_button->set_color(QColor::fromRgbF(color.x, color.y, color.z, color.w));
		m->color_button->set_read_only(entry.attribute(ModelDataAttribute::ReadOnly, false).toBool());
		cache_data();
	}

	void MDEVector4FColor::get_data(ModelDataEntry& entry) const
	{
		auto color = m->color_button->color();
		entry.set_value(QVariant::fromValue(Vector4F{(float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF()}));
	}
}