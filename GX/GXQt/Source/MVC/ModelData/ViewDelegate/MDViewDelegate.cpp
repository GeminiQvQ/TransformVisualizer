// Local Includes
#include <GXQt/MVC/ModelData/ViewDelegate/MDViewDelegate.h>

// Project Includes
#include <GXQt/MVC/ModelData/ViewDelegate/MDViewDelegateEditor.h>
#include <GXQt/MVC/ModelData/ModelData.h>

// Third-Party Includes
#include <QMap>
#include <QPersistentModelIndex>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct MDViewDelegate::Internal
	{
		mutable QMap<QPersistentModelIndex, MDEModelData*> editors;
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	MDViewDelegate::MDViewDelegate(QObject* parent)
		: QStyledItemDelegate {parent}
		, m {std::make_unique<Internal>()}
	{
	}

	MDViewDelegate::~MDViewDelegate()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// QStyledItemDelegate Interface
	//-----------------------------------------------------------------------------------------------------

	QWidget* MDViewDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		auto data_variant = index.data(ModelDataRole);
		if (data_variant.isValid())
		{
			auto data = data_variant.value<ModelData>();
			auto editor = new MDEModelData{parent};
			editor->set_data(data);
			connect(editor, &MDEModelData::signal_data_changed, this, &MDViewDelegate::slot_data_changed);
			connect(editor, &MDEModelData::destroyed, this, &MDViewDelegate::slot_editor_destroyed);
			m->editors.insert(index, editor);
			const_cast<MDViewDelegate*>(this)->sizeHintChanged(index);
			return editor;
		}
		else
		{
			return QStyledItemDelegate::createEditor(parent, option, index);
		}
	}

	void MDViewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
	{
		auto it = m->editors.find(index);
		if (it != m->editors.end())
		{
			auto data = index.data(ModelDataRole).value<ModelData>();
			(*it)->set_data(data);
		}
		else
		{
			return QStyledItemDelegate::setEditorData(editor, index);
		}
	}

	void MDViewDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		auto it = m->editors.find(index);
		if (it != m->editors.end())
		{
			const auto& new_data = (*it)->get_data();
			auto old_data = index.data(ModelDataRole).value<ModelData>();

			if (new_data != old_data)
			{
				if (model->setData(index, QVariant::fromValue(new_data), ModelDataRole))
				{
					auto new_data_2 = model->data(index, ModelDataRole).value<ModelData>();
					if (new_data_2 != new_data)
					{
						(*it)->blockSignals(true);
						(*it)->set_data(new_data_2);
						(*it)->blockSignals(false);
					}
				}
			}
		}
		else
		{
			QStyledItemDelegate::setModelData(editor, model, index);
		}
	}

	QSize MDViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		auto it = m->editors.find(index);
		return (it != m->editors.end() ? (*it)->sizeHint() : QStyledItemDelegate::sizeHint(option, index));
	}

	//-----------------------------------------------------------------------------------------------------
	// Editor Slots
	//-----------------------------------------------------------------------------------------------------

	void MDViewDelegate::slot_data_changed(bool /*committed*/)
	{
		auto editor = static_cast<MDEModelData*>(sender());
		emit commitData(editor);
	}

	void MDViewDelegate::slot_editor_destroyed()
	{
		auto object = sender();

		auto it = std::find_if(m->editors.begin(), m->editors.end(), [object] (auto&& editor)
		{
			return (editor == object);
		});

		emit sizeHintChanged(it.key());

		m->editors.erase(it);
	}
}